Drupal.behaviors.textareaExpander = function() {
  $('textarea.resizable').each(function() {
    $(this).bind("keypress input beforepaste", {currId: this.id}, resizeEventHandler);
    resizeRefresh(this.id); // Initial refresh
  });
  
  $('input.teaser-button').bind("click", resizeRefreshAll); // Fire on a click on the teaser split/join button 

  function resizeEventHandler(event) {
    resizeRefresh(event.data.currId);
  }

  function resizeRefreshAll() { // Split/join teaser has been clicked, refresh textareas
    $('textarea.resizable').each(function() {
      resizeRefresh(this.id);
    });
  }

  function resizeRefresh(currId) {
    resizeTextarea($('textarea#' + currId));
  }

  function resizeTextarea(textarea) {
    var defaultHeight = 150; // We default to 150px.
    var lines = textarea.val().split("\n");
    var count = lines.length;
    $.each(lines, function() { 
      count += parseInt(this.length / 70); 
    });
    var currHeight = parseInt(textarea.css("height"));
    if (!currHeight) { // If no height (i.e. first load), approximate.
      currHeight = (count * 20);
    }
    var rows = parseInt(currHeight / 20) - 1;

    if (count > rows) { // Increase height
      textarea.css("height", (currHeight + (defaultHeight/2)));
    }
    else if (((count + 7) <= rows) && (currHeight > defaultHeight) && (count > 5)) { // Decrease height
      textarea.css("height", (currHeight - (defaultHeight/2)));
    }
    else if ((count <= rows) && (count <= 5)) { // If less than 5 rows, use default height
      textarea.css("height", defaultHeight);
    }
  }
}

