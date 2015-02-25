<?php
/**
 * @file
 * Drush implementation for the wsconfig module.
 */

/**
 * Implementats hook_drush_command().
 */
function wsdata_drush_command() {
  $items = array();
  $items['wsdata-get-endpoint'] = array(
    'description' => dt('Get wsdata endpoint by wsdata config type. This will display the endpoint for the specified type'),
    'aliases' => array('wsdge'),
    'arguments' => array(
      'type' => dt('The type you wish to show the endpoint for, see drush wsdata-list-types.'),
    ),
    'examples' => array(
      'drush wsdata-get-endpoint <wsconfig type machine name>' => dt('Get the endpoint for this wsdata config type.'),
    ),
    'drupal dependencies' => array('wsconfig','wsdata'),
  );
  $items['wsdata-set-endpoint'] = array(
    'description' => dt('Set wsdata endpoint by wsdata config type. This will set the endpoints for all wsconfig of the specified type'),
    'aliases' => array('wsdse'),
    'arguments' => array(
      'type' => dt('The type you wish to update, for list, drush wsdata-list-types.'),
      'uri' => dt('A uri for the endpoint.'),
    ),
    'examples' => array(
      'drush wsdata-set-endpoint <wsconfig type machine name> "http://endpoint/services/rest' => dt('Set the endpoint to http://endpoint/services/rest for this wsdata config type.'),
    ),
    'drupal dependencies' => array('wsconfig','wsdata'),
  );
  $items['wsdata-list-types'] = array(
    'arguments' => array(),
    'description' => dt('Show all wsdata endpoints by uri and endpoint type.'),
    'examples' => array(
      'drush wsdata-list-types' => dt('Show / list summary of all wsdata endpoints.'),
    ),
    'drupal dependencies' => array('wsconfig','wsdata'),
    'aliases' => array('wsdl'),
  );
  return $items;
}

/**
 * Implementats of hook_drush_help().
 */
function wsdata_drush_help($section) {
  switch ($section) {
    case 'drush:wsdata-get-endpoint':
      return dt("This command will get the endpoint of your wsconfig entry by the type of the entry.");
    case 'drush:wsdata-set-endpoint':
      return dt("This command will set the endpoint of your wsconfig entry by the type of the entry.  This is useful when changing your server environments.");
    case 'drush:wsdata-list-types':
      return dt("This command will show pertinent details about wsconfig types including endpoint configured for wsdata.");
  }
}

/**
 * Drush command callback.
 */
function drush_wsdata_set_endpoint() {
  $args = func_get_args();
  $type = $args[0];
  $url = $args[1];  
  
  if (empty($type) or empty($url))  {
    drush_print(dt("Error: Please specify an uri and type of endpoint.  Please see: drush help wsdata-set-endpoint"));
    return drush_set_error('arguments_invalid', dt('Invalid arguments or arguments not provided, please see drush help wsdata-set-endpoint'));
  }

  $wsconfigtype = wsconfig_type_load($type);
  if (!valid_url($url)) {
    return drush_set_error('arguments_invalid', dt('Endpoint url did not pass validation, please see drush help wsdata-set-endpoint'));
  }

  if ($wsconfigtype) {
    $wsconfigtype->setEndpoint($url);
    wsconfig_type_save($wsconfigtype);   
  } else {
    return drush_set_error('arguments_invalid', dt('WSConfig type !type was not found, please select from one of the above or run drush wsdata-list-types', array('!type' => $type)));
  }
}

/**
 * Get a list of all wsdata config types.
 */
function drush_wsdata_list_types() {
  $types = wsconfig_get_types();
  $rows[] = array(dt('Name'), dt('Machine name'), dt('Endpoint'), dt('Connector'));
  foreach ($types as $type) {
    $rows[] = array($type->label, $type->type, $type->data['endpoint'], $type->data['connector']);
  }
  drush_print_table($rows, TRUE);
}

/**
 * Get an endpoint by type.
 */
function drush_wsdata_get_endpoint() {
  if ($args = func_get_args()) {
  } elseif ( empty($args[0]) ) {
    drush_print(dt("Error: Please specify a type.  Please see: drush help wsdata-get-endpoint"));
    return drush_set_error('arguments_invalid', dt('Invalid arguments or arguments not provided, please see drush help wsdata-get-endpoint'));
  }
  $wsconfig_type = wsdata_type_load($args[0]);
  drush_print($wsconfig_type->data['endpoint']);
}