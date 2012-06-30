 $(document).ready(function() {

  for ( var i in BUE.instances ) // Iterate over all BUE instances
  {
    $('textarea#'+BUE.instances[i].textArea.id).bind("keyup", {instance: i}, mdEventHandler); // Fire on keypresses in textareas that belong to this BUE instance
    $('textarea#'+BUE.instances[i].textArea.id).bind("focus", {instance: i}, mdEventHandler); // // Fire on focus (this is also triggered when a change is made from a dialog).
    $('#editor-'+i+' input.bue').bind("click", {instance: i}, mdEventHandler); // // Fire on mouse clicks in this BUE instance's toolbar
    
    // Insert a preview pane
    $('textarea#'+BUE.instances[i].textArea.id+' + div.grippie').after('<div class="markdown-preview" id="markdown-preview-'+BUE.instances[i].textArea.id+'" class="content"><label for="markdown-preview-pane-'+BUE.instances[i].textArea.id+'">'+Drupal.t('Preview:')+'</label><div id="markdown-preview-pane-'+BUE.instances[i].textArea.id+'" name="markdown-preview-pane-'+BUE.instances[i].textArea.id+'"></div></div>');
    mdRefresh(i); // Initial refresh to populate the preview pane
  }
  $('input.teaser-button').bind("click", mdRefreshAllHandler); // Fire on a click on the teaser split/join button 
});

function mdEventHandler(event) {
  mdRefresh(event.data.instance);
}

function mdRefreshAllHandler() { // Split/join teaser has been clicked, refresh all preview panes
  for ( var i in BUE.instances )
  {
    mdRefresh(i);
  }
}

function mdRefresh(instance) { // Re
  var html;
  
  var mdConverter = new Showdown.converter();
  html = mdConverter.makeHtml(BUE.instances[instance].textArea.value); // Make some HTML
  $('div#markdown-preview-pane-'+BUE.instances[instance].textArea.id).after().empty(); // Flush preview pane
  $('div#markdown-preview-pane-'+BUE.instances[instance].textArea.id).append(html);   // ... insert new HTML
}
