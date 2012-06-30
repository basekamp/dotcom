<?php
// $Id: markdowneditor.buttons.inc,v 1.1.2.2 2009/02/04 21:32:11 solipsist Exp $
/*
 * Markdown Editor
 * by Jakob Persson of NodeOne <jakob@nodeone.se>
 *
 * Provides a toolbar for writing Markdown syntax with BUEditor.
 *
 * Sponsored by
 *   NodeOne - www.nodeone.se
 */

$help_url = url('admin/help/markdowneditor', array('absolute' => true));
$imce_url = module_exists('imce') ? url('imce/browse') : '';

$js_helpPath = "markdownEditor.settings.helpPath = '$help_url';";
$js_imceURL = ( $imce_url ? "markdownEditor.settings.IMCEPath = '$imce_url';" : '' );
$js_cssPath = "markdownEditor.settings.cssPath = '". url("$path/library/markdown/markdowneditor.css", array('absolute' => true)) ."';";
$js_addStyleSheet = 'markdownEditor.settings.addStyleSheet();';

// Define buttons
//
// 0: eid
// 1: title
// 2: code/content
// 3: image
// 4: shortcut key
// 5: weight

$mde_buttons = array();

// Increment counter for array keys
$i = 0;

$mde_buttons[$i++] = array(
$eid,
t("Make selected text into a header"),
  "js: $js_helpPath $js_imceURL $js_cssPath $js_addStyleSheet markdownEditor.header();", 
  "md-make-header.gif", 
  "", 
-10
);

$mde_buttons[$i++] = array(
$eid,
t("Italics: Make selected text emphasized"),
  "js: markdownEditor.emphasis();",
  "md-make-em.gif",
  "",
-9
);

$mde_buttons[$i++] = array(
$eid,
t("Make selected text strong"),
  "js: markdownEditor.strongEmphasis();", 
  "md-make-strong.gif", 
  "", 
-8
);

$mde_buttons[$i++] = array(
$eid,
t("Format selected text as code"),
  "js: markdownEditor.codeInline();", 
  "md-make-code.gif", 
  "", 
-7
);

$mde_buttons[$i++] = array(
$eid,
t("Format selected text as a code block"),
  "js: markdownEditor.codeBlock();", 
  "md-make-code-block.gif", 
  "", 
-6
);

$mde_buttons[$i++] = array(
$eid,
t("Make selected text into a block quote"),
    "js: markdownEditor.blockQuote();", 
    "md-make-quote.gif", 
    "", 
-5
);

$mde_buttons[$i++] = array(
$eid,
t("Make selected text into an ordered list (numbered)"),
    "js: markdownEditor.orderedList();", 
    "md-make-ol.gif", 
    "", 
-4
);

$mde_buttons[$i++] = array(
$eid,
t("Make selected text into an unordered list (bullets)"),
    "js: markdownEditor.unorderedList();", 
    "md-make-ul.gif", 
    "", 
-3
);

$mde_buttons[$i++] = array(
$eid,
t("Insert a definition list"),
    "js: $js_helpPath $js_imceURL $js_cssPath $js_addStyleSheet new markdownEditor.DefinitionList();", 
    "md-add-dl.gif", 
    "", 
-2
);

$mde_buttons[$i++] = array(
$eid,
t("Insert a table"),
    "js: $js_helpPath $js_imceURL $js_cssPath $js_addStyleSheet new markdownEditor.Table();", 
    "md-add-table.gif", 
    "", 
-1
);

$mde_buttons[$i++] = array(
$eid,
("Insert an abbreviation (word or acronym with definition)"),
    "js: $js_helpPath $js_imceURL $js_cssPath $js_addStyleSheet markdownEditor.abbreviation();", 
    "md-add-abbreviation.gif", 
    "", 
0
);

$mde_buttons[$i++] = array(
$eid,
t("Insert a footnote"),
    "js: $js_helpPath $js_imceURL $js_cssPath $js_addStyleSheet markdownEditor.footnote();", 
    "md-add-footnote.gif", 
    "", 
1
);

$mde_buttons[$i++] = array(
$eid,
t("Make text into an autolink (turns URLs in links, turns words into section identifiers for navigating the document)"),
    "js: $js_helpPath $js_imceURL $js_cssPath $js_addStyleSheet markdownEditor.autoLink();", 
    "md-add-autolink.gif",
    "",
2
);

$mde_buttons[$i++] = array(
$eid,
t("Make text into a link (turns text into a link with more options)"),
    "js: $js_helpPath $js_imceURL $js_cssPath $js_addStyleSheet markdownEditor.link();", 
    "md-make-link.gif", 
    "", 
3
);

$mde_buttons[$i++] = array(
$eid,
t("Insert an image"),
    "js: $js_helpPath $js_imceURL $js_cssPath $js_addStyleSheet markdownEditor.image();", 
    "md-add-picture.gif", 
    "", 
4
);

$mde_buttons[$i++] = array(
$eid,
t("Insert a line break"),
    "js: $js_helpPath markdownEditor.lineBreak();", 
    "md-add-line.gif", 
    "", 
5
);

$mde_buttons[$i++] = array(
$eid,
t("Insert a horizontal ruler (horizontal line)"),
    "js: $js_helpPath markdownEditor.horizontalRuler();",
    "md-add-hr.gif",
    "",
6
);

$mde_buttons[$i++] = array(
$eid,
t("Help"),
    "js: $js_helpPath $js_imceURL $js_cssPath $js_addStyleSheet markdownEditor.help();", 
    "md-help.gif",
    "",
7
);

// EoF