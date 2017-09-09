/* $Id: filefield_sources.js,v 1.1 2009/04/17 00:20:12 quicksketch Exp $ */

Drupal.behaviors.fileFieldSources = function(context) {
  $('div.filefield-sources-list a', context).click(function() {
    $fileFieldElement = $(this).parents('div.form-item:first').find('div.filefield-element:first');

    // The default upload element is a special case.
    if ($(this).is('.filefield-source-upload')) {
      $fileFieldElement.find('div.filefield-upload').parent().show();
      $fileFieldElement.find('div.filefield-source').hide();
    }
    else {
      $fileFieldElement.find('div.filefield-upload').parent().hide();
      $fileFieldElement.find('div.filefield-source').not('div.' + this.className.replace(' ', '.')).hide();
      $fileFieldElement.find('div.' + this.className.replace(' ', '.')).show();
    }

    // Add the active class.
    $(this).parents('div.filefield-sources-list').find('a.active').removeClass('active');
    $(this).addClass('active');
    Drupal.fileFieldSources.updateHintText($fileFieldElement.get(0));
  });

  // Hide all the other upload mechanisms on page load.
  $('div.filefield-source', context).hide();
  $('div.filefield-sources-list', context).each(function() {
    $(this).find('a:first').addClass('active');
  });
  $('form#node-form', context).submit(function() {
    Drupal.fileFieldSources.removeHintText();
  });
};

/**
 * Helper functions used by FileField Sources.
 */
Drupal.fileFieldSources = {
  /**
   * Update the hint text when clicking between source types.
   */
  updateHintText: function(fileFieldElement) {
    // Add default value hint text to text fields.
    $(fileFieldElement).find('div.filefield-source').each(function() {
      var matches = this.className.match(/filefield-source-([a-z]+)/);
      var sourceType = matches[1];
      var defaultText = '';
      var textfield = $(this).find('input.form-text:first').get(0);
      var defaultText = Drupal.settings.fileFieldSources[sourceType] ? Drupal.settings.fileFieldSources[sourceType].hintText : '';

      // If the field doesn't exist, just return.
      if (!textfield) {
        return;
      }

      // If this field is not shown, remove its value and be done.
      if (!$(this).is(':visible') && textfield.value == defaultText) {
        textfield.value = '';
        return;
      }

      // Set a default value:
      if (textfield.value == '') {
        textfield.value = defaultText;
      }

      // Set a default class.
      if (textfield.value == defaultText) {
        $(textfield).addClass('hint');
      }

      $(textfield).click(hideHintText);
      $(textfield).blur(showHintText);

      function showHintText() {
        if (this.value == '') {
          this.value = defaultText;
          $(this).addClass('hint');
        }
      }

      function hideHintText() {
        if (this.value == defaultText) {
          this.value = '';
          $(this).removeClass('hint');
        }
      }
    });
  },

  /**
   * Delete all hint text from a form before submit.
   */
  removeHintText: function() {
    $('div.filefield-element input.hint').val('').removeClass('hint');
  }
};
