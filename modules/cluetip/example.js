// This is a very simple implementation of using the cluetip jquery
// plugin to change the title of elements with the class "cluetip-title"
// to nice hover overs. For this to work the title has to be in the format
// title="Header|Title text".

$(document).ready(function() {
  $('.cluetip-title[@title]').cluetip({splitTitle: '|'});
});



