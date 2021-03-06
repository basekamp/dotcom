<?php
// $Id: archive.inc,v 1.1.4.3 2010/12/01 03:48:42 winston Exp $

/**
 * @file
 *  This is an archive.org provider include file for Embedded Media Video.
 */

/**
 *  This is the main URL for your provider.
 */
define('EMVIDEO_ARCHIVE_MAIN_URL', 'http://www.archive.org/');

/**
 *  This is the URL to the API of your provider, if this exists.
 */
define('EMVIDEO_ARCHIVE_API_URL', 'http://www.google.com/search?q=archive.org+api');

/**
 *  This defines the version of the content data array that we serialize
 *  in emvideo_archive_data(). If we change the expected keys of that array,
 *  we must increment this value, which will allow older content to be updated
 *  to the new version automatically.
 */
define('EMVIDEO_ARCHIVE_DATA_VERSION', 2);

/**
 * hook emvideo_PROVIDER_info
 * This returns information relevant to a specific 3rd party video provider.
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
function emvideo_archive_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('Yes'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
  );
  return array(
    'provider' => 'archive',
    'name' => t('archive.org'),
    'url' => EMVIDEO_ARCHIVE_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from !archive. You can also read more about its !api.', array('!archive' => l(t('Archive.com'), EMVIDEO_ARCHIVE_MAIN_URL), '!api' => l(t("developer's API"), EMVIDEO_ARCHIVE_API_URL))),
    'supported_features' => $features,
  );
}

/**
 *  hook emvideo_PROVIDER_settings
 *  This should return a subform to be added to the emvideo_settings() admin
 *  settings page.
 *
 *  Note that a form field set will already be provided at $form['archive'],
 *  so if you want specific provider settings within that field set, you should
 *  add the elements to that form array element.
 */
function emvideo_archive_settings() {
  // We'll add a field set of player options here. You may add other options
  // to this element, or remove the field set entirely if there are no
  // user-configurable options allowed by the archive provider.
  /* $form['archive']['player_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embedded video player options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // This is an option to set the video to full screen. You should remove this
  // option if it is not provided by the archive provider.
  $form['archive']['player_options']['emvideo_archive_full_screen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow fullscreen'),
    '#default_value' => variable_get('emvideo_archive_full_screen', 1),
    '#description' => t('Allow users to view video using the entire computer screen.'),
  );

  return $form;*/
}

/**
 *  hook emvideo_PROVIDER_extract
 *
 *  This is called to extract the video code from a pasted URL or embed code.
 *
 *  We'll be passed a URL or the embed code from a video when an editor pastes
 *  that in the field's textfield. We'll need to either pass back an array of
 *  regex expressions to match, or do the matching ourselves and return the
 *  resulting video code.
 *
 *  @param $parse
 *    An optional string with the pasted URL or embed code.
 *  @return
 *    Either an array of regex expressions to be tested, or a string with the
 *    video code to be used. If the hook tests the code itself, it should
 *    return either the string of the video code (if matched), or an empty
 *    array. Otherwise, the calling function will handle testing the embed code
 *    against each regex string in the returned array.
 */
function emvideo_archive_extract($parse = '') {
  // Here we assume that a URL will be passed in the form of
  // http://www.archive.org/video/text-video-title
  // or embed code in the form of <object value="http://www.archive.org/embed...".

  // We'll simply return an array of regular expressions for Embedded Media
  // Field to handle for us.
  return array(
    //In the URL the archive.org item id will appear after "details/" in the URL
    '@archive\.org\/details\/([^\"\&]+)@i',

    // Now we test for embedded video code, which is similar in this case to
    // the above expression, except the item id will appear after "download/"
    '@archive\.org/download/([^/]+)@i',
  );
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_archive_data_version() {
  return EMVIDEO_ARCHIVE_DATA_VERSION;
}

/**
 *  hook emvideo_PROVIDER_data
 *
 *  Provides an array to be serialised and made available with $item elsewhere.
 *
 *  This data can be used to store any extraneous information available
 *  specifically to the archive provider.
 */
function emvideo_archive_data($field, $item, $error_field = '') {

  // Initialize the data array.
  $data = array();

  // Create some version control. Thus if we make changes to the data array
  // down the road, we can respect older content. If allowed by Embedded Media
  // Field, any older content will automatically update this array as needed.
  // In any case, you should account for the version if you increment it.
  $data['emvideo_archive_version'] = $data['emvideo_data_version'] = EMVIDEO_ARCHIVE_DATA_VERSION;

  // Construct a base item url
  $item_url = "http://www.archive.org/details/". $item['value'];
  $download_url = "http://www.archive.org/download/". $item['value'];

  // We will leverage the JSON archive.org api to get details about this item
  // See http://www.archive.org/help/json.php
  $xml_meta_url = $item_url .'&output=json';
  $xml_meta = media_archive_request_xml('archive', $xml_meta_url, array(), TRUE, TRUE, $item['value'] .'_meta', FALSE, TRUE);
  if ($xml_meta['stat'] == 'error' || empty($xml_meta)) {
    drupal_set_message('This item\'s details cannot be retrieved.  The video can not be displayed.');
    return $data;
  }
  else {
    $data['details'] = $xml_meta;
    $server_url = $data['details']['server'] . $data['details']['dir'];
  }

  $item_files = $data['details']['files'];

  if (!$item_files) {
    form_set_error($error_field, 'The list of files for the item at archive.org could not be retrieved.  The video can not be displayed.');
    return $data;
  }

  //Putting the $key (which is actually the file name) as an element of the 
  //array too for convenience later.
  foreach ($item_files as $key => $value) {
    $item_files[$key]['name'] = $key;
  }

  //Get only .mp4 files
  $files_play = array_filter($item_files, "_archive_video_toplay");

  //If there is no playlist then fail
  if (is_null($files_play)) {
    form_set_error($error_field, 'This archive.org item does not appear to have files to play.');
    return $data;
  }

  //Create a playlist suitable for feeding to player
  foreach ($files_play as $file => $file_data) {
    $data['playlist'][] = $download_url . $file;
  }
  
  // Add the thumbnail data.
  $data['thumbnail'] = 'http://www.archive.org/download/'. $item['value'] .'/format=Thumbnail?.jpg';

  return $data;
}

/**
 * Is this a 512Kb MPEG4 file?
 */
function _archive_video_toplay($file) {
  if ($file['format'] == "512Kb MPEG4") {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * hook emvideo_PROVIDER_duration($item)
 * Returns the duration of the video in seconds.
 *  @param $item
 *    The video item itself, which needs the $data array.
 *  @return
 *    The duration of the video in seconds.
 */
function emvideo_archive_duration($item) {
  if (!isset($item['data']['emvideo_archive_version'])) {
    $item['data'] = emvideo_archive_data(NULL, $item);
  }
  return isset($item['data']['metadata']['RUNTIME'][0]) ? emvideo_convert_to_seconds($item['data']['metadata']['RUNTIME'][0]) : 0;
}

/**
 *  hook emvideo_PROVIDER_rss
 *
 *  This attaches a file to an RSS feed.
 */
function emvideo_archive_rss($item, $teaser = NULL) {
  if ($item['value']) {
    $file['thumbnail']['filepath'] = $item['data']['thumbnail'];

    return $file;
  }
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site.
 *  @param $video_code
 *    The string containing the video to watch.
 *  @return
 *    A string containing the URL to view the video at the original provider's site.
 */
function emvideo_archive_embedded_link($video_code) {
  return 'http://www.archive.org/details/'. $video_code;
}

/**
 * The embedded flash displaying the archive video.
 */
function theme_emvideo_archive_flash($item, $width, $height, $autoplay) {
  $output = '';
  if ($item['embed']) {
    $flowplayerplaylist = _media_archive_flowplayer_playlist($item['data']['playlist']);
    $controlplaylist = (count($item['data']['playlist'])>1) ? "true" : "false";
    $clipautoplay = $autoplay ? 'true' : 'false';
    $output = <<<EOD
      <embed type="application/x-shockwave-flash" width="$width"   height="$height"  allowfullscreen="true"
        allowscriptaccess="always"  src="http://www.archive.org/flow/flowplayer.commercial-3.0.5.swf"
        w3c="true"  flashvars='config={
          "key":"#\$b6eb72a0f2f1e29f3d4",
          "playlist":[
            {
              "url":"{$item['data']['thumbnail']}",
              "autoPlay":true,
              "scaling":"fit"
            },
            {$flowplayerplaylist}
          ],
          "clip":{
            "autoPlay":false,
            "accelerated":true,
            "scaling":"fit"
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
              "fullscreen":true,
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
              "{$item['value']}":"function()"
            },
            "-",
            "Flowplayer 3.0.5"
          ]
        }'> </embed>
EOD;
  }
  return $output;
}

/**
 * hook emvideo_PROVIDER_thumbnail
 * Returns the external url for a thumbnail of a specific video.
 *  @param $field
 *    The field of the requesting node.
 *  @param $item
 *    The actual content of the field from the requesting node.
 *  @return
 *    A URL pointing to the thumbnail.
 */
function emvideo_archive_thumbnail($field, $item, $formatter, $node, $width, $height) {
  // In this demonstration, we previously retrieved a thumbnail using oEmbed
  // during the data hook.
  return $item['data']['thumbnail'];
}

/**
 *  hook emvideo_PROVIDER_video
 *  This actually displays the full/normal-sized video we want, usually on the
 *  default page view.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emvideo_archive_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_archive_flash', $item, $width, $height, $autoplay);
  return $output;
}

/**
 *  hook emvideo_PROVIDER_video
 *
 *  This actually displays the preview-sized video we want, commonly for the
 *  teaser.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emvideo_archive_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_archive_flash', $item, $width, $height, $autoplay);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme().
 * This returns any theme functions defined by this provider.
 */
function emvideo_archive_emfield_subtheme() {
    $themes = array(
        'emvideo_archive_flash'  => array(
            'arguments' => array('item' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
            'file' => 'providers/archive.inc',
            // If you don't provide a 'path' value, then it will default to
            // the emvideo.module path. Obviously, replace 'emarchive' with
            // the actual name of your custom module.
            //'path' => drupal_get_path('module', 'emarchive'),
        )
    );
    return $themes;
}

/**
 *  Implement hook_emvideo_PROVIDER_content_generate().
 */
function emvideo_archive_content_generate() {
  return array(
    'http://www.archive.org/details/DrupalconDc2009-DrupalMultimedia',
    'http://www.archive.org/details/DrupalconBoston2008-DrupalMultimedia',
    'http://www.archive.org/details/drupal_song_dcamp_pune_09',
    'http://www.archive.org/details/Drupal.Peru_DrupalSong20090718',
  );
}
