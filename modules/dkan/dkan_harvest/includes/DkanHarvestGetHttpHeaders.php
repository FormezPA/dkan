<?php

/**
 * @file
 * Class DkanHarvestGetHttpHeaders: retrive a remote file headers.
 */

/**
 * Get the headers of a remote file without get the file's body.
 */
class DkanHarvestGetHttpHeaders {

  const METHOD_CURL = 'curl';
  const METHOD_FSOCKET = 'fsocket';
  const METHOD_PHP = 'php';

  /**
   * Public method: get the header of $url and return an associative array.
   *
   *    The function returns an array,
   *    like the get_headers() function with the format=1.
   *    It works in the following order:
   *    - curl method as preferred
   *    - fsocket as fallback
   *    - get_headers as second fallback.
   *
   * @see http://php.net/manual/en/function.get-headers.php
   *
   * @param string $url
   * @param string $force_method
   *
   * @return array
   */
  public static function get_headers($url, $force_method = NULL) {

    // First choice: curl.
    if (function_exists('curl_init')
      && (empty($force_method) || $force_method == self::METHOD_CURL)) {

      return self::_get_headers_curl($url);
    }
    elseif (function_exists('fsockopen')
      && (empty($force_method) || $force_method == self::METHOD_FSOCKET)) {

      // Second choiche fsockopen.
      return self::_get_headers_with_fsocket($url);
    }
    else {
      // Third choiche: function get_headers.
      return get_headers($url, 1);
    }
  }

  /**
   * Get the headers from file with cURL.
   *
   * @param mixed $url
   *
   * @return mixed
   */
  protected static function _get_headers_curl($url) {

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HEADER, TRUE);
    curl_setopt($ch, CURLOPT_NOBODY, TRUE);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_TIMEOUT, 5);

    $r = curl_exec($ch);

    return self::_parse_header_response($r);

  }

  /**
   * Get headers with the fsocket method.
   *
   * @param string $url
   *
   * @return array
   *
   * @throws Exception
   */
  protected static function _get_headers_with_fsocket($url) {

    $purl = parse_url($url);
    $port = (isset($purl['port'])) ? $purl['port'] : '80';
    $timeout = '10';

    switch ($purl['scheme']) {
      case 'https':
        $scheme = 'ssl://';
        $port = 443;
        break;

      case 'http':
      default:
        $scheme = '';
        $port = 80;
    }

    try {
      if (!$fp = fsockopen($purl['host'], $port, $errNo, $errMsg, $timeout)) {
        throw new Exception($errMsg, $errNo);
      }
    }
    catch (Exception $ex) {
      drush_log($ex->getMessage(), 'warning');
      return array();
    }

    // HEAD or GET ?
    $requestheader = "GET " . $purl['path'] . " HTTP/1.1\r\n";
    $requestheader .= "HOST: " . $purl['host'] . "\r\n\r\n";
    fputs($fp, $requestheader);

    $response = '';
    while (!feof($fp)) {
      $chunk = fgets($fp);
      if ($chunk == "\r\n") {
        break;
      }
      else {
        $response .= $chunk;
      }
    }

    fclose($fp);
    return self:: _parse_header_response($response);
  }

  /**
   * Parse the response of cURL and fsocket method.
   *
   * @param string $str_response
   *
   * @return array
   */
  private static function _parse_header_response($str_response) {

    $r = explode("\n", $str_response);

    $data = array();

    foreach ($r as $value) {
      $tk = explode(": ", $value, 2);
      if (trim($tk[0]) == '') {
        continue;
      }
      elseif (count($tk) == 2) {
        $data[$tk[0]] = trim($tk[1]);
      }
      else {
        $data[] = trim($tk[0]);
      }
    }

    return $data;
  }

}
