<?php

/**
 * @file
 * Administrative callbacks for ltv migration.
 */

function ltv_migration_gallery_import() {
  $form['show_galleries_csv_filename'] = array(
    '#type' => 'textfield',
    '#title' => t('Show Galleries CSV filename'),
    '#required' => TRUE,
    '#description' => t('CSV filename only.'),
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Import'));

  return $form;
}

/**
 * Submit handler.
 */
function ltv_migration_gallery_import_submit($form, &$form_state) {
  $show_galleries_csv_filename = $form_state['values']['show_galleries_csv_filename'];
  if ($show_galleries_csv_filename) {
    $filename = "http://lifetime:tv4w0men@devwww.mylifetime.com/d6/scripts/movies-migration/" . $show_galleries_csv_filename;
    $row = 0;
    $fields_row =array();
    if (($handle = fopen($filename, "r")) !== FALSE) {
      while (($data = fgetcsv($handle)) !== FALSE) {
        if($row == 0){
          $fields_row = $data;
        } else {
          dsm($data); dsm($fields_row);
          //_gallery_movie_node_save($data , $fields_row);
        }
        $row++;
      }
      fclose($handle);
    }
    drupal_set_message(t("Total Gallery Node Imported: !row", array('!row' =>($row-1))));
  }
  else {
    drupal_set_mesage(t("Pass the Batch ID to Argument"));
  }
}


function _gallery_movie_node_save($node_row , $fields_row){
  $frame_nid_ref = array();
  if (count($node_row) > 0) {
    foreach($fields_row as $key => $field){
      $node[trim($field)] = $node_row[$key];
    }

    // Prepare gallery node.
    $gallery_node = new stdClass();
    $gallery_node->type = 'photo_gallery';
    $gallery_node->title = _remove_filter($node['title']);
    $gallery_node->status = 1;
    $gallery_node->uid = 1;
    $gallery_node->comment = 0;
    $gallery_node->language = LANGUAGE_NONE;
    $gallery_node->body[LANGUAGE_NONE][0]['value'] = _remove_filter($node['field_short_description']);
    $gallery_node->body[LANGUAGE_NONE][0]['format'] = 'wysiwyg';

    // Set path alias for gallery node.
    $split_path = explode('/',$node['path']);
    $node['new_path'] = $node['path'];
    if(count($split_path)>=4){
      $node['new_path'] = str_replace('/photos/','/pictures/',$node['path']);
    }else if(count($split_path)<=3){
      $node['new_path'] = str_replace('/photos','/pictures/',$node['path']);
      $node['new_path'] = $node['new_path'].'/'.$split_path[1].'-photos';
    }
    $gallery_node->path['alias'] = $node['new_path'];
    $gallery_node->path['pathauto'] = TRUE;

    // Set gallery node thumbnail.
    if(!empty($node['field_gallery_node_thumbnail'])) {
      $field = field_info_field('field_gallery_thumbnail');
      $instance = field_info_instance('node', 'field_gallery_thumbnail', 'content_type');

      $dest = file_field_widget_uri($field,$instance);

      $path = "http://www.mylifetime.com/d6/".$node['field_gallery_node_thumbnail'];
      $file_temp = file_get_contents($path);
      $filename = basename($path);

      // Saves a file to the specified destination and creates a database entry.
      $file_temp = file_save_data($file_temp, $dest .'content_type/photo-gallery/'. $filename, FILE_EXISTS_RENAME);
      $gallery_node->field_gallery_thumbnail = array(
        LANGUAGE_NONE => array(
          0 => array(
          'fid' => $file_temp->fid,
          'filename' => $file_temp->filename,
          'filemime' => $file_temp->filemime,
          'uid' => 1,
          'uri' => $file_temp->uri,
          'status' => 1,
          'display' => 1
          )
          )
      );
    }

    // Set parent movie node.
    $movie_nid ='';
    if(count($split_path)>0){
      $slit_path1 = array_slice($split_path, 0, 2);
      $movie_path = implode('/', $slit_path1);
      $src_path = db_query("SELECT source FROM url_alias where alias = :alias", array(':alias' => $movie_path))->fetchField();

      if($src_path){
         $path_split = explode('/',$src_path);
         $movie_nid = $path_split[1];
      }
    }
    if($movie_nid) {
      $gallery_node->field_parent_movie_node[LANGUAGE_NONE][0]['nid'] = $movie_nid;
    }

    // Set gallery images for gallery.
    if ($frame_nid_ref = _photo_frame_save($node)) {
      foreach($frame_nid_ref as $key => $nid){
        $gallery_node->field_gallery_images[LANGUAGE_NONE][$key]['nid'] = $nid;
      }

      // Save the gallery node.
      if ($_gallery_node = node_submit($gallery_node)) {
        node_save($gallery_node);
        $gallery_nid = $_gallery_node->nid;
      }
      else {
        drupal_set_message("Gallery Node ".$_frame_node->title."title added incorrectly");
      }

      // Update the gallry image node with paresent gallery.
      if($gallery_nid) {
        foreach($frame_nid_ref as $key => $nid) {
          $frame_node = node_load($nid);
          $frame_node->field_parent_gallery[LANGUAGE_NONE][0]['nid'] = $gallery_nid;
          node_save($frame_node);
        }
      }
    }
    else {
      drupal_set_message("Node ".$gallery_node->title." added incorrectly. No gallery image found.");
    }
    drupal_set_message("Photo Gallery Node Title " .$node['title']. " Created");
  }
  else {
    drupal_set_message("Empty Record Set");
  }
}

function _photo_frame_save($node) {
	$frame_nid_ref = array();
	$frame_images = explode(';',$node['field_frame_image']);
	$frame_titles = explode(';',$node['frame_title']);
	$frame_captions = explode(';',$node['field_frame_caption']);
	$frame_credits = explode(';',$node['field_frame_credits']);
	$frame_node_thumbnail = explode(';',$node['field_frame_node_thumbnail']);
	//$split_path = explode('/',$node['path']);
	//$slit_path1 = array_slice($node['path'], 0, 2);
	//echo count($frame_images);
  // Loop through each image and save it.
	for($i=0;$i<count($frame_images);$i++) {
	  $frame_node = new stdClass();
	  $picture = $i+1;
	  $frame_node->type = 'photo_gallery_image';
	  $frame_node->title = _remove_filter($frame_titles[$i]);
	  $frame_node->status = 1;
	  $frame_node->uid = 1;
	  $frame_node->comment = 0;
	  $frame_path = $node['new_path'];
	  $split_path = explode('/',$node['new_path']);

    // Set path alias for gallery image node.
	  if(count($split_path)>=4){
  	  $frame_path = $node['new_path'].'/'.$split_path[3].'-'.$picture;
	  }
	  $frame_node->path['alias'] = $frame_path;
  	$frame_node->path['pathauto'] = TRUE;

	  $frame_node->language = LANGUAGE_NONE;
	  $frame_node->field_caption[LANGUAGE_NONE][0]['value'] = _remove_filter($frame_captions[$i]);
	  $frame_node->field_credits[LANGUAGE_NONE][0]['value'] = _remove_filter($frame_credits[$i]);

    // Set tumbnail image to gallery image node.
	  if(!empty($frame_node_thumbnail[$i])) {
      $field = field_info_field('field_node_thumbnail');
      $instance = field_info_instance('node', 'field_photo_thumbnail', 'content_type');

      $dest = file_field_widget_uri($field,$instance);

	    $path = "http://www.mylifetime.com/d6/".$frame_node_thumbnail[$i];
	    $file_temp = file_get_contents($path);
      $filename = basename($path);

	    //Saves a file to the specified destination and creates a database entry.
      $file_temp = file_save_data($file_temp, $dest .'content_type/photo-gallery-image/'. $filename, FILE_EXISTS_RENAME);
	    $frame_node->field_photo_thumbnail = array(
        LANGUAGE_NONE => array(
          0 => array(
            'fid' => $file_temp->fid,
            'filename' => $file_temp->filename,
            'filemime' => $file_temp->filemime,
            'uid' => 1,
            'uri' => $file_temp->uri,
            'status' => 1,
            'display' => 1,
          )
        )
      );
    }

    // Set the image to gallery image node.
	  if(!empty($frame_images[$i])){
      $field = field_info_field('field_photo_large');
      $instance = field_info_instance('node', 'field_photo_large', 'content_type');

      $dest = file_field_widget_uri($field,$instance);

	    $path = "http://www.mylifetime.com/d6/".$frame_images[$i];
	    $file_temp = file_get_contents($path);
      $filename = basename($path);

	    //Saves a file to the specified destination and creates a database entry.
      $file_temp = file_save_data($file_temp, $dest .'content_type/photo-gallery-image/'. $filename, FILE_EXISTS_RENAME);
	    $frame_node->field_photo_large = array(
        LANGUAGE_NONE => array(
          0 => array(
            'fid' => $file_temp->fid,
            'filename' => $file_temp->filename,
            'filemime' => $file_temp->filemime,
            'uid' => 1,
            'uri' => $file_temp->uri,
            'status' => 1,
            'display' => 1
          )
	      )
      );
	  }

    // Submit the gallery image node.
	  if ($_frame_node = node_submit($frame_node)) {
      node_save($frame_node);
      $frame_nid_ref[] = $_frame_node->nid;
	  }
    else {
      drupal_set_message("Frame Node ".$_frame_node->title." added incorrectly");
	  }
	}

	return $frame_nid_ref;
}

function _remove_filter($string){
  $string = str_replace("newlinerep", "\r\n", $string);
  $string = str_replace("commarep", ",", $string);
  return $string;
}