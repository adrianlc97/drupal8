<?php

/**
 * @file
 * ip2country module integration with Drush 8 and earlier.
 *
 * Provides three drush commands:
 * - ip2country-update to update the database.
 * - ip2country-lookup to look up an IP address.
 * - ip2country-status to display current update status.
 */

/**
 * Implements hook_drush_command().
 */
function ip2country_drush_command() {
  $items = [];
  $items['ip2country-update'] = [
    'aliases' => ['ip-update'],
    'description' => dt('Updates the Ip2Country database from the selected Regional Internet Registry.'),
    'drupal dependencies' => ['ip2country'],
    'core' => ['8+'],
    'options' => [
      'registry' => [
        'description' => dt('Registry used to obtain data.'),
        'example-value' => 'afrnic, apnic, arin, lapnic, or ripe',
      ],
      'md5' => [
        'description' => dt('Validate data integrity with MD5 checksum.'),
      ],
      'batch_size' => [
        'description' => dt('Row insertion batch size. Defaults to 200 rows per insert.'),
        'example-value' => 200,
      ],
    ],
    'examples' => [
      'drush ip-update --registry=ripe' => dt('Updates Ip2Country database of ip/country associations.'),
      'drush ip-update --registry=ripe --batch_size=200 --md5' => dt('Updates Ip2Country database of ip/country associations.'),
    ],
  ];
  $items['ip2country-lookup'] = [
    'aliases' => ['ip-lookup'],
    'description' => dt('Returns the country name and two-character ISO 3166 country code associated with the given IP address.'),
    'drupal dependencies' => ['ip2country'],
    'core' => ['8+'],
    'arguments' => [
      'IPV4' => dt('The IPV4 address to look up, in dotted-quad notation (e.g. 127.0.0.1).'),
    ],
    'required-arguments' => TRUE,
    'examples' => [
      'drush ip-lookup IPV4' => dt('Returns a country code associated with the given IP address.'),
      'drush ip-lookup IPV4 --fields=ip_address,name --no-field-labels' => dt('Returns the IP address and Country name.'),
    ],
    // Command returns a single keyed data item
    // (e.g. ['country_code_iso_2' => 'UK']) that can either be printed with a
    // label (e.g. 'Country: UK'), or output raw with --pipe (e.g. 'UK').
    'outputformat' => [
      'default' => 'table',
      'pipe-format' => 'list',
      'field-labels' => [
        'ip_address' => (string) dt('IP address'),
        'name' => (string) dt('Country'),
        'country_code_iso2' => (string) dt('Country code'),
      ],
      'output-data-type' => 'format-table',
    ],
  ];
  $items['ip2country-status'] = [
    'aliases' => ['ip-status'],
    'description' => dt('Displays the time and RIR of the last database update.'),
    'drupal dependencies' => ['ip2country'],
    'core' => ['8+'],
    'examples' => [
      'drush ip-status' => dt('Displays the time and RIR of the last database update.'),
    ],
  ];

  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * Use for extensive help, more than just the short usage summary that
 * is already provided in hook_drush_command().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'.
 */
function ip2country_drush_help($section) {
  switch ($section) {
    // The 'title' meta item is used to name a group of
    // commands in `drush help`.
    case 'meta:ip2country:title':
      return dt('Ip2Country commands');

    // The 'summary' meta item is displayed in `drush help --filter`,
    // and is used to give a general idea what the commands in this
    // command file do, and what they have in common.
    case 'meta:ip2country:summary':
      return dt("Identifies a user's location based on IP address.");

    // The ip2country-lookup command.
    case 'drush:ip2country-lookup':
      return dt('Returns the 2-character ISO 3166 country code associated with a given IP address.');

    // The ip2country-update command.
    case 'drush:ip2country-update':
      return dt('Updates the Ip2Country database from the selected Regional Internet Registry.');

    // The ip2country-status command.
    case 'drush:ip2country-status':
      return dt('Displays the time and RIR of the last database update.');
  }
}

/**
 * Implements drush_hook_COMMAND_validate() for ip2country-lookup.
 *
 * Requires user to specify one IPV4 on the command line.
 */
function drush_ip2country_lookup_validate() {
  $ipv4 = drush_get_arguments();
  // $ipv4[0] is the command name, $ipv4[1] is the required argument.
  if (!isset($ipv4[1])) {
    drush_set_error(
      'DRUSH_IP2COUNTRY_MISSING_ARG',
       dt('Usage: drush ip2country-lookup IPV4' . PHP_EOL . PHP_EOL . "Try 'drush help ip2country-lookup' for more information.")
    );
  }
}

/**
 * Implements drush_hook_COMMAND() for the ip2country-update command.
 *
 * Updates the database from the selected registry.
 */
function drush_ip2country_update() {
  $ip2country_config = \Drupal::config('ip2country.settings');
  $watchdog = $ip2country_config->get('watchdog');

  $registry = (string) drush_get_option('registry', $ip2country_config->get('rir'));
  $md5_checksum = (bool) drush_get_option('md5', $ip2country_config->get('md5_checksum'));
  $batch_size = (int) drush_get_option('batch_size', $ip2country_config->get('batch_size'));

  drush_print(dt('Updating ... '), 0, NULL, FALSE);

  $status = \Drupal::service('ip2country.manager')->updateDatabase($registry, $md5_checksum, $batch_size);
  if ($status != FALSE) {
    drush_print(dt('Completed.'));
    drush_print(dt('Database updated from @registry server. Table contains @rows rows.', [
      '@registry' => mb_strtoupper($registry),
      '@rows' => $status,
    ]));
    // Log update to watchdog, if ip2country logging is enabled.
    if ($watchdog) {
      \Drupal::logger('ip2country')->notice('Drush-initiated database update from @registry server.', [
        '@registry' => mb_strtoupper($registry),
      ]);
    }
  }
  else {
    drush_print(dt('Failed.'));
    drush_print(dt('Database update from @registry server FAILED.', [
      '@registry' => mb_strtoupper($registry),
    ]));
    // Log update failure to watchdog, if ip2country logging is enabled.
    if ($watchdog) {
      \Drupal::logger('ip2country')->warning('Drush-initiated database update from @registry server FAILED.', [
        '@registry' => mb_strtoupper($registry),
      ]);
    }
  }

}

/**
 * Implements drush_hook_COMMAND() for the ip2country-lookup command.
 *
 * Gets the ISO 3166 2-character country code from the IP address.
 *
 * @param string|int $ip_address
 *   IP address either as a dotted quad string (e.g. '127.0.0.1') or
 *   as a 32-bit unsigned long integer.
 */
function drush_ip2country_lookup($ip_address) {
  $country_code = \Drupal::service('ip2country.lookup')->getCountry($ip_address);

  if ($country_code == FALSE) {
    drush_print(dt('IP address not found in the database.'));
  }
  else {
    $country_list = \Drupal::service('country_manager')->getList();
    $country_name = $country_list[$country_code];
    return [
      $ip_address => [
        'ip_address' => $ip_address,
        'name' => (string) dt($country_name),
        'country_code_iso2' => $country_code,
      ],
    ];
  }
}

/**
 * Displays the time and RIR of the last database update.
 */
function drush_ip2country_status() {
  $stateService = \Drupal::service('state');
  $update_time = $stateService->get('ip2country_last_update');
  if (!empty($update_time)) {
    $dateFormatter = \Drupal::service('date.formatter');
    $message = dt(
      'Database last updated on @date at @time from @registry server.',
      [
        '@date' => $dateFormatter->format($update_time, 'ip2country_date'),
        '@time' => $dateFormatter->format($update_time, 'ip2country_time'),
        '@registry' => mb_strtoupper($stateService->get('ip2country_last_update_rir')),
      ]
    );
  }
  else {
    $message = dt('Database is empty.');
  }

  drush_print($message);
}
