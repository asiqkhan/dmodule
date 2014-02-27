<?php

/**
 * @file
 * The theme system, which controls the output of simple node archive.
 *
 * Set archive variable for template.
 */

function _sna_blocks_preprocess_sna_blocks(&$vars) {
  try {
    $results = $vars['view']->result;
    if (!empty($results)) {
      //check for node id, node title and date created
      if (!isset($vars['view']->result[0]->node_title)) {
        drupal_set_message(t('Add node title'));
      }
      $results = _sna_blocks_resultset($results);
      $archive_page = $vars['options']['sna_view_page_url'];
      $use_jquerymenu = isset($vars['options']['sna_use_jquery']) && $vars['options']['sna_use_jquery'] ? $vars['options']['sna_use_jquery'] : 0;
      $arg = explode('/', $_GET['q']);
      $display_year = $arg[0] == $archive_page && isset($arg[1]) && $arg[1] != '' && is_numeric($arg[1]) ? $arg[1] : '';
      $display_month = $arg[0] == $archive_page && isset($arg[2]) && $arg[2] != '00' && is_numeric($arg[2]) ? date('F', mktime(0, 0, 0, $arg[2])) : '';
      $vars['archive_block'] = _sna_blocks_archivehtml($results, $display_year, $display_month, $archive_page, $use_jquerymenu);
    }
    else {
      $vars['archive_block'] = t('No Result found');
    }
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage());
  }
}

/**
 * Format views results to add year, month and count.
 *
 * @param array $results
 *   Array contain views result.
 *
 * @return array
 *   An associate arrry.
 */

function _sna_blocks_resultset($results) {
  $archive = array();
  // Adding year and month field.
  foreach ($results as $row) {
    $archive[date('Y', $row->node_created)][date('F', $row->node_created)][$row->nid] = $row->node_title;
  }
  // Adding count field.
  if (!empty($archive)) {
    foreach ($archive as $year => $month) {
      $year_count = 0;
      foreach ($month as $key => $title) {
        $count = count($title, COUNT_RECURSIVE);
        $archive[$year][$key . ' (' . $count . ')'] = $archive[$year][$key];
        unset($archive[$year][$key]);
        $year_count += $count;
      }
      $archive[$year . ' (' . $year_count . ')'] = $archive[$year];
      unset($archive[$year]);
    }
  }

  return $archive;
}

/**
 * Theme the result set to HTML.
 *
 * @param array $results
 *   Associate array in format
 *   Array([year(counter)] => Array([month(counter)] => Array([nid] => title)))
 * @param string $display_year
 *   Which year to be shown in archive.
 *   A full numeric representation of a year, 4 digit.
 * @param string $display_month
 *   Which month to be shown in arhive.
 *   Numeric representation of a month, with leading zeros.
 * @param string $archive_page
 *   View name.
 * @param string $use_jquerymenu
 *   Options to use jquerymenu 0 or 1
 *
 * @return string
 *   An HTML string.
 */

function _sna_blocks_archivehtml($results, $display_year, $display_month, $archive_page, $use_jquerymenu) {
  $output = t('No Content Posted.');
  $archive_items = 0;
  if ($node = menu_get_object()) {
    $node_created = $node->created;
    $display_year = date('Y', $node_created);
    $display_month = date('F', $node_created);
  }

  $year_items = $trail = array();


  // Build the HTML.
  if (!empty($results)) {
    $theme_arr = array();
    // Loop through earch year.
    foreach ($results as $year => $months) {
      $year_value = explode(' ', $year);
      // $year_value[0] contain year value and $year_value[1] conatian count.
      $year_url = $archive_page . '/' . $year_value[0];

      // Check for active year branch and block.
      $expand_year = $display_year == $year_value[0] ? TRUE : FALSE;

      $children_months = array();
      // If Using Jquery Menu.
      if ($use_jquerymenu) {
        unset($year_item);
        unset($month_items);
        $year_item['link']['href'] = $year_url;
        $year_item['link']['title'] = t($year_value[0]) . ' ' . $year_value[1];
        $year_item['link']['has_children'] = 1;
        $year_item['link']['expanded'] = 0;
        $year_item['link']['options']['attributes'] = array();
        $year_item['link']['hidden'] = 0;
        if ($expand_year) {
          $trail[0] = $year_item['link']['href'];
        }
        $expand_year = TRUE;
      }

      if ($expand_year) {
        // Loop through active year and show month.
        foreach ($months as $month => $nodes) {
          $month_value = explode(' ', $month);
          // $month_value[0] contain month and $month_value[1] conatian count.
          $month_url = $archive_page . '/' . $year_value[0] . '/' . date('m', strtotime($month_value[0]));

          // Check for active month and block.
          $expand_month = $display_year == $year_value[0] && $display_month == $month_value[0] ? TRUE : FALSE;

          $children_nodes = array();
          // If Using Jquery Menu.
          if ($use_jquerymenu) {
            unset($month_item);
            unset($node_items);
            $month_item['link']['href'] = $month_url;
            $month_item['link']['title'] = t($month_value[0]) . ' ' . $month_value[1];
            $month_item['link']['has_children'] = 1;
            $month_item['link']['expanded'] = 0;
            $month_item['link']['options']['attributes'] = array();
            $month_item['link']['hidden'] = 0;
            if ($expand_month) {
              $trail[0] = $year_item['link']['href'];
              $trail[1] = $month_item['link']['href'];
            }
            $expand_month = TRUE;
          }

          if ($expand_month) {
            $count = 1;
            // Loop through acitive month and show node.
            foreach ($nodes as $nid => $title) {
              // If Using Jquery Menu.
              if ($use_jquerymenu) {
                unset($node_item);
              }
              // $archive_items number of nodes display in expanded archive.
              if ($count <= $archive_items || $archive_items == 0) {
                $children_nodes[] = l($title, 'node/' . $nid);
                // If Using Jquery Menu.
                if ($use_jquerymenu) {
                  $node_item['link']['href'] = 'node/' . $nid;
                  $node_item['link']['title'] = $title;
                  $node_item['link']['has_children'] = 0;
                  $node_item['link']['expanded'] = 0;
                  $node_item['link']['options']['attributes'] = array();
                  $node_item['link']['hidden'] = 0;
                }
              }
              $count = $count != 0 ? $count + 1 : 0;

              // If Using Jquery Menu.
              if ($use_jquerymenu) {
                $node_items[] = $node_item;
              }
            }

            // If Using Jquery Menu.
            if ($use_jquerymenu) {
              $month_item['below'] = $node_items;
              $month_items[] = $month_item;
            }
          }
          $children_months[] = array(
            'data' => l(t($month_value[0]), $month_url) . '&nbsp;<span>' . $month_value[1] . '</span>',
            'children' => $children_nodes,
          );
        }

        // If Using Jquery Menu.
        if ($use_jquerymenu) {
          $year_item['below'] = $month_items;
          $year_items[] = $year_item;
        }
      }
      $theme_arr[] = array(
        'data' => l(t($year_value[0]), $year_url) . '&nbsp;<span>' . $year_value[1] . '</span>',
        'children' => $children_months,
      );
    }

    // If Using Jquery Menu.
    if ($use_jquerymenu) {
      $output = theme('jquerymenu_menu', array('tree' => $year_items, 'trail' => $trail));
    }
    else {
      $output = theme('item_list', array('items' => $theme_arr));
    }
  }
  else {
    // Term is not tagged with any node.
    $output = t('No posts available');
  }

  return $output;
}