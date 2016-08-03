<?php

  /**
   * @file
   * Drush file for the Islandora Datastream Editor module.
   */

  /**
  * Implements hook_drush_command().
  */
  function islandora_datastream_editor_drush_command() {
    $items = array();
    $items['islandora_datastream_editor_property'] = array(
      'aliases' => array('idesp'),
      'description' => 'Edit a datastream property of a PID.',
      'examples' => array(
        'drush islandora_datastream_editor_property --user=admin --PID=yul:123 --DSID=OBJ --label="THIS IS MY NEW LABEL" --mimetype="application/x-iso9660-image"'
      ),
      'options' => array(
        'PID' => array(
          'description' => 'The PID of the object you want to update.',
        ),
        'DSID' => array(
          'description' => 'The DSID of the object you want to update.',
        ),
        'label' => array(
          'description' => 'The label of the DSID you want to update.',
        ),
        'mimetype' => array(
          'description' => 'The mimetype of the DSID you want to update.',
        ),
      ),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
    );
    return $items;
  }

  /**
   * Updates the properties on a given datastream for a given PID.
   */
  function drush_islandora_datastream_editor_property() {
     module_load_include('inc', 'islandora', 'includes/utilities');
     drupal_static_reset('islandora_get_tuque_connection');
     $de_pid = drush_get_option('PID');
     $de_dsid = drush_get_option('DSID');
     $de_label = drush_get_option('label');
     $de_mimetype = drush_get_option('mimetype');

     if (!drush_confirm(dt('Updating datastream label to (!de_label) or datastream mimetype to (!de_mimetype) on (!de_dsid) for (!de_pid).',
       array('!de_pid' => $de_pid, '!de_label' => $de_label, '!de_dsid' => $de_dsid, '!de_mimetype' => $de_mimetype))));

     // Load the pid.
     if (!$object = islandora_object_load($de_pid)) {
       drush_set_error('IBD_OBJECT_NOT_FOUND',
         dt('The specified object (!de_pid) does not exist or is not accessible.',
           array('!de_pid' => $de_pid)));
         continue;
     }
     // Update the datastream label.
     if (drush_get_option('label')) {
       $ds = $object[$de_dsid];
       $ds->label = $de_label;
       $object->ingestDatastream($ds);
       drush_log(dt('Updating datastream label to (!de_label) on (!de_dsid) for (!de_pid).',
         array('!de_pid' => $de_pid, '!de_label' => $de_label, '!de_dsid' => $de_dsid)));
     }
     // Update the datastream mimetype.
     if (drush_get_option('mimetype')) {
       $ds = $object[$de_dsid];
       $ds->mimetype = $de_mimetype;
       $object->ingestDatastream($ds);
       drush_log(dt('Updating datastream mimetype to (!de_label) on (!de_dsid) for (!de_pid).',
         array('!de_pid' => $de_pid, '!de_label' => $de_label, '!de_dsid' => $de_dsid)));
     }
  }
