 $(document).ready(function() {

    $('textarea.markdown-preview').each(function() {
      $('textarea#'+this.id).bind("keyup", {currId: this.id}, mdEventHandler);
      $('textarea#'+this.id+' + div.grippie').after('<div class="markdown-preview" id="markdown-preview-'+this.id+'" class="content"><label for="markdown-preview-pane-'+this.id+'">'+Drupal.t('Preview:')+'</label><div id="markdown-preview-pane-'+this.id+'" name="markdown-preview-pane-'+this.id+'"></div></div>');
      // Initial refresh to populate the preview pane
      mdRefresh(this.id);
    });
  $('input.teaser-button').bind("click", mdRefreshAllHandler); // Fire on a click on the teaser split/join button 
});

function mdEventHandler(event) {
  mdRefresh(event.data.currId);
}

function mdRefreshAllHandler() { // Split/join teaser has been clicked, refresh all preview panes
  $('textarea.markdown-preview').each(function() {
    mdRefresh(this.id);
  });
}

function mdRefresh(currId) { // Re
  var html;
  
  var mdConverter = new Showdown.converter();
  html = mdConverter.makeHtml($('textarea#'+currId).val()); // Make some HTML
  $('div#markdown-preview-pane-'+currId).after().empty(); // Flush preview pane
  $('div#markdown-preview-pane-'+currId).append(html);   // ... insert new HTML
}
