<?php
// $Id: archive_audio.inc,v 1.2.4.3 2010/12/01 03:48:42 winston Exp $

/**
 * @file
 *   This include processes archive.org audio files for use with Embedded Media Field.
 */

/**
 *  This is the main URL for your provider.
 */
define('EMAUDIO_ARCHIVE_MAIN_URL', 'http://www.archive.org/');

/**
 *  This is the URL to the API of your provider, if this exists.
 */
define('EMAUDIO_ARCHIVE_API_URL', 'http://www.google.com/search?q=archive.org+api');

/**
 *  This defines the version of the content data array that we serialize
 *  in emaudio_archive_data(). If we change the expected keys of that array,
 *  we must increment this value, which will allow older content to be updated
 *  to the new version automatically.
 */
define('EMAUDIO_ARCHIVE_DATA_VERSION', 2);

/**
 * hook emaudio_PROVIDER_info
 * This returns information relevant to a specific 3rd party audio provider.
 *
 * @return
 *   A keyed array of strings requested by various admin and other forms.
 *    'provider' => The machine name of the provider. This must be the same as
 *      the base name of this filename, before the .inc extension.
 *    'name' => The translated name of the provider.
 *    'url' => The url to the main page for the provider.
 *    'settings_description' => A description of the provider that will be
 *      posted in the admin settings form.
 *    'supported_features' => An array of rows describing the state of certain
 *      supported features by the provider. These will be rendered in a table,
 *      with the columns being 'Feature', 'Supported', 'Notes'. In general,
 *      the 'Feature' column will give the name of the feature, 'Supported'
 *      will be Yes or No, and 'Notes' will give an optional description or
 *      caveats to the feature.
 */
function emaudio_archive_audio_info() {
  $features = array(
    array(t('Thumbnails'), t('No'), ''),
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS attachment'), t('No'), ''),
  );
  return array(
    'provider' => 'archive_audio',
    'name' => t('Archive'),
    'url' => EMAUDIO_ARCHIVE_MAIN_URL,
    'settings_description' => t('These settings specifically affect audio played from !archive. You can also read more about its !api.', array('!archive' => l(t('Archive.com'), EMAUDIO_ARCHIVE_MAIN_URL), '!api' => l(t("developer's API"), EMAUDIO_ARCHIVE_API_URL))),
    'supported_features' => $features,
  );
}

/**
 *  hook emaudio_PROVIDER_settings
 *  This should return a subform to be added to the emaudio_settings() admin
 *  settings page.
 *
 *  Note that a form field set will already be provided at $form['archive'],
 *  so if you want specific provider settings within that field set, you should
 *  add the elements to that form array element.
 */
function emaudio_archive_audio_settings() {
  /* No Settings for this provider */
}

/**
 *  hook emaudio_PROVIDER_extract
 *
 *  This is called to extract the audio code from a pasted URL or embed code.
 *
 *  We'll be passed a URL or the embed code from a audio when an editor pastes
 *  that in the field's textfield. We'll need to either pass back an array of
 *  regex expressions to match, or do the matching ourselves and return the
 *  resulting audio code.
 *
 *  @param $parse
 *    An optional string with the pasted URL or embed code.
 *  @return
 *    Either an array of regex expressions to be tested, or a string with the
 *    audio code to be used. If the hook tests the code itself, it should
 *    return either the string of the audio code (if matched), or an empty
 *    array. Otherwise, the calling function will handle testing the embed code
 *    against each regex string in the returned array.
 */
function emaudio_archive_audio_extract($parse = '') {
  // Here we assume that a URL will be passed in the form of
  // http://www.archive.org/details/text-audio-title
  // or embed code in the form of
  //<embed ... "playlist":[{"url":"http://www.archive.org/download/text-audio-title/...

  // We'll simply return an array of regular expressions for Embedded Media
  // Field to handle for us.
  return array(
    //In the URL the archive.org item id will appear after "details/" in the URL
    '@archive\.org\/details\/([^\"\&]+)@i',

    // Now we test for embedded audio code, which is similar in this case to
    // the above expression, except the item id will appear after "download/"
    '@archive\.org/download/([^/]+)@i',
  );
}

/**
 *  Implement hook emaudio_PROVIDER_data_version().
 */
function emaudio_archive_audio_data_version() {
  return EMAUDIO_ARCHIVE_DATA_VERSION;
}

/**
 *  hook emaudio_PROVIDER_data
 *
 *  Provides an array to be serialised and made available with $item elsewhere.
 *
 *  This data can be used to store any extraneous information available
 *  specifically to the archive provider.
 */
function emaudio_archive_audio_data($field, $item, $error_field = '') {

  // Initialize the data array.
  $data = array();

  // Create some version control. Thus if we make changes to the data array
  // down the road, we can respect older content. If allowed by Embedded Media
  // Field, any older content will automatically update this array as needed.
  // In any case, you should account for the version if you increment it.
  $data['emaudio_archive_version'] = EMAUDIO_ARCHIVE_DATA_VERSION;

  // Construct a base item url
  $item_url = "http://www.archive.org/details/". $item['value'];

  // We will leverage the JSON archive.org api to get details about this item
  // See http://www.archive.org/help/json.php
  $xml_meta_url = $item_url .'&output=json';
  $xml_meta = media_archive_request_xml('archive', $xml_meta_url, array(), TRUE, TRUE, $item['value'] .'_meta', FALSE, TRUE);
  if ($xml_meta['stat'] == 'error' || empty($xml_meta)) {
    drupal_set_message('This item\'s details cannot be retrieved.  The audio can not be displayed.');
    return $data;
  }
  else {
    $data['details'] = $xml_meta;
    $server_url = $data['details']['server'] . $data['details']['dir'];
  }

  $item_files = $data['details']['files'];

  if (!$item_files) {
    form_set_error($error_field, 'The list of files for the item at archive.org could not be retrieved.  The audio can not be displayed.');
    return $data;
  }

  //Putting the $key (which is actually the file name) as an element of the 
  //array too for convenience later.
  foreach ($item_files as $key => $value) {
    $item_files[$key]['name'] = $key;
  }

  //Get playlist files only (.m3u files)
  $files_playlists = array_filter($item_files, "_archive_audio_isplaylist");

  //If there is no playlist then fail
  if (is_null($files_playlists)) {
    form_set_error($error_field, 'This archive.org item does not appear to have a playlist.');
    return $data;
  }

  //We'll custom sort the array so we'll get first VBR playlists, then other 
  //playlists sorted by highest bitrate.  Goal is to present the highest 
  //bitrate playlist offered for this item.
  $data['playlists'] = $files_playlists;
  $worked = uasort($files_playlists, "_archive_audio_playlistsort");
  $candidate_playlist = array_shift($files_playlists);
  $data['playlist_file_used'] = $candidate_playlist;

  //Retrieve the playlist
  $result = drupal_http_request('http://' . $server_url . $candidate_playlist['name']);
  if (!empty($result->error)) {
    form_set_error($error_field, 'The playlist for the item at archive.org could not be retrieved.  The audio can not be displayed.');
    return $data;
  }
  $playlist = $result->data;
  //We'll store it in the data element as it is used in our theme function to 
  //output the player later.
  $data['playlist'] = explode("\n", trim($playlist));

  return $data;
}

/**
 * Sort playlist files
 */
function _archive_audio_playlistsort($file1, $file2) {
  $components1 = explode(" ", $file1['format']);
  $components2 = explode(" ", $file2['format']);
  if ($components1[0] == $components2[0]) {
    return 0;
  }
  //We'll prefer the Variable Bitrate (VBR)
  elseif ($components1[0] == "VBR") {
    return -1;
  }
  elseif ($components2[0] == "VBR") {
    return 1;
  }
  //Otherwise we'll look for the highest bitrate playlist
  $br1 = (int)$components1[0];
  $br2 = (int)$components2[0];
  if ($br1 > $br2) {
    return -1;
  }
  else {
    return 1;
  }
}

/**
 * Is this file a playlist (.m3u) file?
 */
function _archive_audio_isplaylist($file) {
  if (substr($file['format'], -3, 3) == "M3U") {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * hook emfield_PROVIDER_rss
 */
function emaudio_archive_audio_rss($item, $teaser = NULL) {
  // Get size and mime type of first playlist file
  $url = $item['data']['playlist'][0];
  $response = emfield_request_header('archive_audio', $url, $cached = FALSE);
  if ($response->code == 200) {
    $data['size'] = $response->headers['Content-Length'];
    $data['mime'] = $response->headers['Content-Type'];
  }

  if ($data['size']) {
    $file = array();
    $file['filepath'] = $url;
    $file['filesize'] = $data['size'];
    $file['filemime'] = $data['mime'];
  }
  return $file;
}

/**
 * hook emaudio_PROVIDER_embedded_link($audio_code)
 * returns a link to view the audio at the provider's site.
 *  @param $audio_code
 *    The string containing the audio item.
 *  @return
 *    A string containing the URL the audio item at the original provider's site.
 */
function emaudio_archive_audio_embedded_link($audio_code) {
  return 'http://www.archive.org/details/'. $audio_code;
}

/**
 * Implementation of hook emaudio_archive_audio_audio().
 *
 * This actually displays the full/normal-sized audio we want, usually on the default page view.
 *
 * @param $embed
 *   The audio code for the audio to embed.
 * @param $width
 *   The width to display the audio.
 * @param $height
 *   The height to display the audio.
 * @param $field
 *   The field info from the requesting node.
 * @param $item
 *   The actual content from the field.
 * @return
 *   The html of the embedded audio.
 */
function emaudio_archive_audio_audio($embed = NULL, $width = 0, $height = 0, $field = NULL, $item, $node, $autoplay) {
  $output = theme('emaudio_archive_audio_flash', $embed, $width, $height, $field, $item, $node, $autoplay);

  return $output;
}

/**
 * The embedded flash displaying the archive audio.
 */
function theme_emaudio_archive_audio_flash($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = '';
  if ($item) {
    $flowplayerplaylist = _media_archive_flowplayer_playlist($item['data']['playlist']);
    $controlplaylist = (count($item['data']['playlist'])>1) ? "true" : "false";
    $clipautoplay = $autoplay ? 'true' : 'false';
    $output = <<<EOD
<embed type="application/x-shockwave-flash" width="$width" height="$height" allowfullscreen="true"
  allowscriptaccess="always" src="http://www.archive.org/flow/flowplayer.commercial-3.0.5.swf" w3c="true"
  flashvars='config={
    "key":"#\$b6eb72a0f2f1e29f3d4",
    "playlist":[
    {$flowplayerplaylist}
    ],
    "clip":{
        "autoPlay":{$clipautoplay}
      },
    "canvas":{
      "backgroundColor":"0x000000",
      "backgroundGradient":"none"
    },
    "plugins":{
      "audio":{
        "url":"http://www.archive.org/flow/flowplayer.audio-3.0.3-dev.swf"
      },
      "controls":{
        "playlist":{$controlplaylist},
        "fullscreen":false,
        "gloss":"high",
        "backgroundColor":"0x000000",
        "backgroundGradient":"medium",
        "sliderColor":"0x777777",
        "progressColor":"0x777777",
        "timeColor":"0xeeeeee",
        "durationColor":"0x01DAFF",
        "buttonColor":"0x333333",
        "buttonOverColor":"0x505050"
      }
    },
    "contextMenu":[
      {
        "Item {$item['value']} at archive.org":"function()"
      },
      "-",
      "Flowplayer 3.0.5"
    ]
  }'>
</embed>
EOD;
  }
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme().
 */
function emaudio_archive_audio_emfield_subtheme() {
  return array(
    'emaudio_archive_audio_flash'  => array(
            'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'field' => NULL, 'data' => NULL, 'node' => NULL, 'autoplay' => NULL),
            'file' => 'providers/emaudio/archive_audio.inc',
            'path' => drupal_get_path('module', 'media_archive'),
        )
    );
}
