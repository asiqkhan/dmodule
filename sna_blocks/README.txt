This module provide simple node archive style option for Views.

The archive blocks can be theme using Jquery Menu Module.
For the archive blocks you can set the number of nodes will display under month.

This module depends on views(https://drupal.org/project/views) module.

-- INSTALLATION --

  Install module like normal

-- CONFIGURATION --

  Go to your edit view. Select format as 'Simple Node Archive' and then settings
    * View machine name :
      View machine name to which the archive block will attach.
    * View page display I D:
      View page display id and the archive block will attache to that page.
    * sna_view_page_url:
      Attach page url.
    * Field Name:
      Achive will created based on field. Require <a href="http://drupal.org/project/date" target="_blank">Date module </a>.
      Add the same field in views fields list.
    * Use Jquerymenu Module:
      Check this box if you want to use Jquerymenu module to theme
      archive blocks.
