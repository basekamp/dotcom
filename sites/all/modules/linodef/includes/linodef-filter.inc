<?php
// $Id: linodef-filter.inc,v 1.1.2.11 2009/07/05 21:51:46 roidanton Exp $

/**
 * @file
 * The replace functions executed in the process filter step.
 *
 * @package Linodef
 * @author Roi Danton
 */

/**
 * Return body with replaced tags.
 *
 * Search and replace the tags with internal links and node title or field values.
 *
 * @param $body
 *   Text on that the preg_replace function will be executed.
 */
function _linodef_filter_process(&$body) {

	// Define Linodef tags
	$preg = array(

		// No options therefore comment has to be treated separately.
		// Link Node ID and embed title or field values.
		"@\[#([0-9]+)\]@e"                                      => "_linodef_find_nodesnfields($1);",
		"@\[#([0-9]+):([0-9a-z_]+)\]@e"                         => "_linodef_find_nodesnfields($1,$2);",
		"@\[#([0-9]+):([0-9a-z_]+):([0-9]+)\]@e"                => "_linodef_find_nodesnfields($1,$2,$3);",

		// Link Node ID and embed title or field values, comment will be substituted.
		"@\[#([0-9]+?) [^\]\r\n]+\]@e"                          => "_linodef_find_nodesnfields($1);",
		"@\[#([0-9]+?):([0-9a-z_]+?) [^\]\r\n]+\]@e"            => "_linodef_find_nodesnfields($1,$2);",
		"@\[#([0-9]+?):([0-9a-z_]+?):([0-9]+?) [^\]\r\n]+\]@e"  => "_linodef_find_nodesnfields($1,$2,$3);",

		// Embed view.
		"@\[#([0-9a-zA-Z_]+?)\]@e"                              => "_linodef_find_view($1);",
		"@\[#([0-9a-zA-Z_]+?) [^\]\r\n]+\]@e"                   => "_linodef_find_view($1);",

		// ID with options, alphanumeric for viewname, possible comment will be substituted.
		// Including backwards compatibility for RC2 option-value syntax without "": (?:=[a-zA-Z]*)*.
		'@\[#([0-9a-zA-Z_]+),((?:[,0-9a-zA-Z]*(?:="[^"\]\r\n]*")*(?:=[a-zA-Z]*)*)*)[^\]\r\n]*\]@e'                 => '_linodef_filter_processoptions($1,False,0,"$2");',
		'@\[#([0-9]+):([0-9a-z_]+),((?:[,0-9a-zA-Z]*(?:="[^"\]\r\n]*")*(?:=[a-zA-Z]*)*)*)[^\]\r\n]*\]@e'           => '_linodef_filter_processoptions($1,$2,0,"$3");',
		'@\[#([0-9]+):([0-9a-z_]+):([0-9]+),((?:[,0-9a-zA-Z]*(?:="[^"\]\r\n]*")*(?:=[a-zA-Z]*)*)*)[^\]\r\n]*\]@e'  => '_linodef_filter_processoptions($1,$2,$3,"$4");',

		// Inline comment which would not appear for readers, only editors.
		'@\[# [^\]\r\n]+]@e'  => '',

		// Backwards compatibility for RC1/2 syntax.
		// Link Node ID with user defined text.
		"@\xf4\x80\x81\x8c#([0-9]+)\xf4\x80\x8f\x8d(.*?)\xf4\x80\x81\x8c/#\xf4\x80\x8f\x8d@se"                => "_linodef_find_node($1,'$2');",
		// Embed title or field values, add no link.
		"@\[#nl([0-9]+)\]@e"                                    => "_linodef_filter_processoptions($1,False,0,'nolink');",
		"@\[#nl([0-9]+):([0-9a-z_]+)\]@e"                       => "_linodef_filter_processoptions($1,$2,0,'nolink');",
		"@\[#nl([0-9]+):([0-9a-z_]+):([0-9]+)\]@e"              => "_linodef_filter_processoptions($1,$2,$3,'nolink');",
		// Link term ID and embed term name.
		"@\[#t([0-9]+)\]@e"                                     => "_linodef_find_term($1);",

	);
	$body = preg_replace(array_keys($preg), array_values($preg), $body);

	return $body;
}

/**
 * Helper function for _linodef_filter_process.
 * Process options, determine the tag element type and launch the function to retrieve the data.
 *
 * @param $id
 *   ID of the tag.
 * @param string $fieldname
 *   Name of a field if set, else false.
 * @param $valuenumber
 *   Number to get the desired value of multiple value fields or 0 if not set.
 * @return
 *   Returns a string containing the link or a hint if node not found.
 */
function _linodef_filter_processoptions($id, $fieldname = FALSE, $valuenumber = 0, $option_string = '') {
	$type = '';
	$message = '';
	if (!empty($option_string)) {

		$options = linodef_create_valid_tag_option_array($option_string);

		if (is_array($options)) {
			// 1. Get and validate ID type if given: Last occurence determines ID type (if user does the fault including several).
			if ($options['id']) {
				foreach ($options['id'] as $idtype => $value) {
					if (!$fieldname || $idtype == 'nid') {
						$type = $idtype;
					}
					// If fieldname with different idtype set.
					else {
						$type = 'nid';
						$message .= linodef_message(array('warning' => array('option ignored' => array('%id' => $id, '%option' => $idtype)), 'explanation' => array('option fieldname set' => TRUE)), TRUE);
					}
				}
			}
			// 2. Get and validate options. If ID type not given previously, determine ID type.
			if ($options['options']) {
				foreach ($options['options'] as $option => $value) {
					$properties = $options['valid_options'][$option];
					$compatible_option = FALSE;

					// a. Check if option is compatible with id type.
					if ($type) {
						if (array_key_exists($type, $properties['id_types'])) {
							$compatible_option = TRUE;
						}
						else {
							$message .=  linodef_message(array('warning' => array('option ignored' => array('%id' => $id, '%option' => $option)), 'explanation' => array('option not compatible with idtype' => array('%idtype' => $type, '%option' => $option))), TRUE);
						}
					}
					// Try to determine the best fitting type. First fit determines ID type.
					elseif (!array_key_exists('nid', $properties['id_types'])) {
						$compatible_option = TRUE;
						reset($properties['id_types']);
						$type = key($properties['id_types']);
					}
					// Catch nid options if no type is given.
					else {
						$compatible_option = TRUE;
					}

					// b. Check for empty values.
					if ($compatible_option) {
						if ($properties['value'] === FALSE) {
							$validated_options[$option] = $value;
						}
						elseif ($value !== '' && $value !== NULL) {
							$validated_options[$option] = $value;
						}
						else {
							$message .= linodef_message(array('warning' => array('option ignored' => array('%id' => $id, '%option' => $option)), 'explanation' => array('empty value' => TRUE)), TRUE);
						}
					}
				}
			}
			// If no type is set (e.g. no or global tag options like nolink or linktext are used), set type to nid.
			if (!$type) {
				$type = 'nid';
			}

			// Create messages for empty and unknown options.
			if ($options['unknown']) {
				foreach ($options['unknown'] as  $key => $value) {
					if ($key === '') {
						$message .= linodef_message(array('custom' => t('Enter a further option for Linodef tag with id %id or remove the last comma inside the tag', array('%id' => $id))), TRUE, 'warning');
					}
					else {
						$message .= linodef_message(array('warning' => array('option unknown' => array('%id' => $id, '%option' => $key))), TRUE);
					}
				}
			}
		}
	}
	// An empty option string shouldn't have called this function unless there is a needless comma left.
	else {
		$message .= linodef_message(array('custom' => t('Enter an option for Linodef tag with id %id or remove the comma inside the tag', array('%id' => $id))), TRUE, 'warning');
		$options = array();
	}
	// Output.
	switch ($type) {
		case 'nid':
			return _linodef_find_nodesnfields($id, $fieldname, $valuenumber, $validated_options) . $message;
			break;
		case 'tid':
			return _linodef_find_term($id, $validated_options) . $message;
			break;
		case 'viewid':
			return _linodef_find_view($id, $validated_options) . $message;
			break;
	}
}

/**
 * Helper function for _linodef_filter_process.
 * VIEW REFERENCE, IMAGE, FILE implementation should make their display settings optional, e.g. image= Generic files, Image, Path to File etc
 *
 * @param string $nid
 *   Node ID of the node that will be linked.
 * @param string $fieldname
 *   The name of the field reading the value of or "title" (to include nodetitle) if not set.
 * @param int $valuenumber
 *   Number to get the desired value of multiple value fields or 0 if not set.
 * @param array $options
 *   key is option name, value is option value
 *   - no link, linktext
 *   - translation: If set the function will check if the node has a translated node in the language this value has (e.g. de, en, es etc).
 * @return
 *   Returns a string containing the (linked) node title/field value or a hint if node/field/field-content not found.
 */
function _linodef_find_nodesnfields($nid, $fieldname = FALSE, $valuenumber = 0, $options = array()) {
	if ($nid && $node = node_load($nid)) {
		// If translation option is set, find translated node with desired language. If no node in that language is available continue as usual.
		if (isset($options['translation']) && $language = $options['translation']) {
			if (module_exists('translation')) {
				if ($tnid = $node->tnid) {
					if ($translations = translation_node_get_translations($tnid)) {
						foreach ($translations as $value) {
							if ($value->language == $language) {
								$node = node_load($value->nid);
							}
						}
					}
				}
			}
			else {
				return linodef_message(array('warning' => array('activate module' => 'content translation'), 'explanation' => array('option to use in tag id' => array('%option' => 'translation', '%id' => $nid))), TRUE);
			}
		}
		if (node_access('view', $node)) {
			if ($fieldname == FALSE) {
				$output = $node->title;
			}
			elseif (property_exists($node, $fieldname)) {
				// Heed content permissions settings if module is enabled.
				if (module_exists('content_permissions')) {
					$content_perm = user_access('view '. $fieldname) ? TRUE : FALSE;
					if (!$content_perm) {
						return linodef_message(array('error' => array('field' => $fieldname, 'type' => 'access denied')), TRUE);
					}
				}
				$output = get_object_vars($node);
				// Get field values.
				$output = $output[$fieldname][$valuenumber]['value'];
				// Check if field values are set, otherwise output message.
				if (drupal_strlen($output) < 1) {
					$erroneous_tag[0] = '#'. $nid .':'. $fieldname;
					// Set correct tag layout for error message heeding the translation option.
					if ($options['translation'] && $nid != $node->nid) {
						$erroneous_tag[2] = ',translation='. $options['translation'];
					}
					if ($valuenumber == 0) {
						$erroneous_tag_str = ksort($erroneous_tag) ? implode($erroneous_tag) : '';
						return linodef_message(array('custom' => t('Field %field of node %nid has no content and hence cannot be embedded. Create content for that field or revise the Linodef tag which contains %erroneous_tag', array('%field' => $fieldname, '%nid' => $node->nid, '%erroneous_tag' => $erroneous_tag_str))), TRUE, 'error');
					}
					else {
						$erroneous_tag[1] = ':'. $valuenumber;
						$erroneous_tag_str = ksort($erroneous_tag) ? implode($erroneous_tag) : '';
						return linodef_message(array('custom' => t('Value %valuenumber of field %field of node %nid not found or has no content and hence cannot be embedded. Create content for that field or revise the Linodef tag which contains %erroneous_tag', array('%valuenumber' => $valuenumber, '%field' => $fieldname, '%nid' => $node->nid, '%erroneous_tag' => $erroneous_tag_str))), TRUE, 'error');
					}
				}
			}
			else {
				return linodef_message(array('error' => array('field' => $fieldname, 'type' => 'not found'), 'custom' => t('or is not assigned to content type of node %nid', array('%nid' => $nid))), TRUE);
			}

			// Remove existing Linodef tags before doing markup to avoid self and cross referencing (causing an infinite recursion) until unlimited depths.
			$output = linodef_removetags($output);
			// Remove self referencing tags causing an infinite recursion and thus white page. Not needed, recursion removed.
			// $output = str_replace('[#'. $nid .':'. $fieldname .']', '', $output);
			// How to remove cross referencing tags causing an infinite recursion and thus white page? Possible till a depth of 2 with variable_set, but cross ref still possible with deeper depth. Therefore recursion removed.

			if (isset($options['nolink'])) {
				return $output;
			}
			else {
				if (isset($options['linktext'])) {
					$output = $options['linktext'];
				}
				$class = $fieldname ? 'linodef linodef_field linodef_field_'. $fieldname : 'linodef linodef_node linodef_node_'. $node->nid;
				// l() does check_plain on $text and does any path aliases if required (thx @AjK). html => true let l() doesn't do check_plain.
				return l($output, 'node/'. $node->nid, array('html' => true, 'attributes' => array('title' => t('Node') .' '. $node->nid .': '. $node->title, 'class' => $class)));
			}
		}
		else {
			// Access denied needed: Otherwise imagine the following: If a user writes a comment and tries to include e.g. [#NodeID] he would be able to see the title even if he have no proper access rights.
			return linodef_message(array('error' => array('node' => $node->nid, 'type' => 'access denied')), TRUE);
		}
	}
	else {
		return linodef_message(array('error' => array('node' => $nid, 'type' => 'not found')), TRUE);
	}
}

/**
 * Helper function for _linodef_filter_process.
 *
 * @param $tid
 *   Term ID of the term that will be linked and embedded.
 * @param $options
 *   - no link, linktext
 * @return
 *   Returns a string containing the linked term name or a hint if term not found.
 */
function _linodef_find_term($tid, $options = array()) {
	if (module_exists('taxonomy')) {
		if ($tid && $term = taxonomy_get_term($tid)) {
			if (isset($options['nolink'])) {
				return check_plain($term->name);
			}
			$termlink = taxonomy_term_path($term);
			if (isset($options['linktext'])) {
				$output = $options['linktext'];
			}
			else {
				$output = $term->name;
			}
			return l($output, $termlink, array('attributes' => array('title' => t('Shows a teaser list of nodes using term !term', array('!term' => $term->name)), 'class' => 'linodef linodef_term linodef_term_'. $term->name)));
		}
		else {
			return linodef_message(array('error' => array('term' => $tid, 'type' => 'not found')), TRUE);
		}
	}
	else {
		return linodef_message(array('warning' => array('activate module' => 'Taxonomy'), 'explanation' => array('option to use in tag id' => array('%option' => 'tid', '%id' => $tid))), TRUE);
	}
}

/**
 * Helper function for _linodef_filter_process.
 *
 * @param $viewid
 *   View ID of the view that will be linked and embedded.
 * @param $options
 *   - no link, linktext
 *   - viewdisplay: Use a certain view display.
 *   - viewargs: Pass arguments to the view.
 *   - viewtitle: Include view title and link to the view.
 * @return
 *   Returns a string containing the linked view name or a message.
 */
function _linodef_find_view($viewid, $options = array()) {
	if (module_exists('views')) {
		if ($viewid && $view = views_get_view($viewid)) {
			$display = $options['viewdisplay'] && !is_numeric($options['viewdisplay']) ? $options['viewdisplay'] : 'default';
			$args = isset($options['viewargs']) ? explode('&', $options['viewargs']) : array();

			// Embed whole view, currently without link to view.
			if (!$options['viewtitle'] && !$options['linktext'] ) {
				return $view->preview($display, $args);
			}
			if (isset($options['linktext'])) {
				$output = $options['linktext'];
			}
			else {
				// Show view display title.
				$output = $view->get_title() ? $view->get_title() : t('view %viewname', array('%viewname' => $view->name));
			}
			// Create link if available/needed.
			if (($path = $view->display[$display]->display_options['path']) && !$view->disabled && !isset($options['nolink'])) {
				$url_args = implode('/', $args);
				if ($url_args) {
					$path .= '/'. $url_args;
				}
				return l($output, $view->get_url($args, $path), array('html' => true, 'attributes' => array('title' => t('Show !view', array('!view' => $output)), 'class' => 'linodef linodef_view linodef_view_'. $output)));
			}
			else {
				return $output;
			}
		}
		else {
			return linodef_message(array('error' => array('view' => $viewid, 'type' => 'not found')), TRUE);
		}
	}
	else {
		return linodef_message(array('warning' => array('activate module' => 'Views'), 'explanation' => array('option to use in tag id' => array('%option' => 'viewid', '%id' => $viewid))), TRUE);
	}
}

/**
 * Helper function for _linodef_filter_process.
 * This function is marked as deprecated. Option linktext offers this feature.
 *
 * @param $nid
 *   Node ID of the node that will be linked.
 * @param string $text
 *   The user entered text.
 * @return
 *   Returns a string containing the link or a hint if node not found.
 */
function _linodef_find_node($nid, $text) {
	if ($nid && $node = node_load($nid)) {
		if (node_access('view', $node)) {
			// l() does check_plain on $text and does any path aliases if required (thx @AjK).
			return l($text, 'node/'. $nid, array('attributes' => array('title' => $node->title)));
		}
		else {
			// Access denied not needed because the user without required rights won't see any content from the node he has no access to.
			return check_plain($text);
		}
	}
	return linodef_message(array('error' => array('node' => $nid, 'type' => 'not found'), 'custom' => t('desired to linking %usertext', array('%usertext' => $text))), TRUE);
}
