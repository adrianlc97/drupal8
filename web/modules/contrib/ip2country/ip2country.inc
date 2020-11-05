<?php

/**
 * @file
 * Utility routines to load the IP to Country database.
 *
 * Data is obtained from one of the five Regional Internet Registries
 * (AFRINIC, ARIN , APNIC, LACNIC, or RIPE).
 *
 * Data format is described by ftp://ftp.arin.net/pub/stats/arin/README
 *
 * This code derived from "class eIp2Country" posted at
 * http://www.tellinya.com/read/2007/06/03/ip2country-translate-ip-address-to-country-code/
 */

/**
 * Updates the database.
 *
 * Truncates ip2country table then reloads from ftp servers.
 *
 * @param string $registry
 *   Regional Internet Registry from which to get the data.
 *   Allowed values are afrinic, apnic, arin (default), lacnic, or ripe.
 *
 * @return int|false
 *   FALSE if database update failed. Otherwise, returns the number of
 *   rows in the updated database.
 *
 * @deprecated in ip2country:8.x-1.9 and is removed from ip2country:8.x-2.0.
 *   Use the ip2country.manager service instead. For example,
 *   \Drupal::service('ip2country.manager')->updateDatabase($rir).
 *
 * @see https://www.drupal.org/node/3076484
 */
function ip2country_update_database($registry = 'arin') {
  @trigger_error("ip2country_update_database() is deprecated in ip2country:8.x-1.9 and is removed from ip2country:8.x-2.0. Use \Drupal::service('ip2country.manager')->updateDatabase() instead. See https://www.drupal.org/node/3076484", E_USER_DEPRECATED);
  $ip2country_config = \Drupal::config('ip2country.settings');
  $md5_checksum = $ip2country_config->get('md5_checksum');
  $batch_size = $ip2country_config->get('batch_size');
  return \Drupal::service('ip2country.manager')->updateDatabase($registry, $md5_checksum, $batch_size);
}

/**
 * Empties the ip2country table in the database.
 *
 * @deprecated in ip2country:8.x-1.9 and is removed from ip2country:8.x-2.0.
 *   Use the ip2country.manager service instead. For example,
 *   \Drupal::service('ip2country.manager')->emptyDatabase().
 *
 * @see https://www.drupal.org/node/3076484
 */
function ip2country_empty_database() {
  @trigger_error("ip2country_empty_database() is deprecated in ip2country:8.x-1.9 and is removed from ip2country:8.x-2.0. Use \Drupal::service('ip2country.manager')->emptyDatabase() instead. See https://www.drupal.org/node/3076484", E_USER_DEPRECATED);
  \Drupal::service('ip2country.manager')->emptyDatabase();
}
