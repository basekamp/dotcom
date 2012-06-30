Markdown Preview provides a preview pane that displays the rendered HTML output of your Markdown input. 

Markdown Preview consists of two submodules:

Markdown Preview - Provides a preview pane for textarea form fields.

Markdown Preview for BUEditor - Provides a preview pane for BUEditor. This works best with solipsist's markdowneditor add-on for BUEditor (http://drupal.org/project/markdowneditor).
Note that the Showdown Markdown implementation that Markdown Preview uses doesn't support Markdown Extra. You may wish to disable the unsupported toolbar buttons in BUEditor.



Installation instructions
-------------------------

Markdown Preview:

1. Download and Showdown and copy showdown.js to the "showdown" directory inside the markdownpreview module directory.
2. Download and install Markdown filter module: http://drupal.org/project/markdown
3. Enable the module and change the settings as needed: admin/settings/markdownpreview

Markdown Preview for BUEditor:
1. Download and Showdown and copy showdown.js to the "showdown" directory inside the markdownpreview module directory.
2. Download, install and configure the required modules:
   - Markdown filter module: http://drupal.org/project/markdown
   - BUEditor: http://drupal.org/project/bueditor
   - Markdown Editor: http://drupal.org/project/markdowneditor
3. Enable the module and change the settings as needed: admin/settings/markdownpreview_bueditor


Requirements
------------
Showdown - a JavaScript Markdown implementation by John Fraser. Link: http://attacklab.net/showdown/
Markdown - Link: http://drupal.org/markdown

