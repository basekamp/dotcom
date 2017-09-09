// $Id: notifications_nice.js,v 1.1.2.1 2009/04/28 13:25:54 jareyero Exp $

/**
 * Adds subscription widgets to nodes, works with notifications_nice module
 *
 * Code from http://groups.drupal.org/node/17779
 */
Drupal.clueTipAttach = function() {
  $('a.load-local').cluetip({local:true, sticky:true, closePosition: 'title', cursor: 'pointer'});
  $("html > *").not("a").click(function() {$("#cluetip-close").triggerHandler("click");});
}

if (Drupal.jsEnabled) {
  $(document).ready(Drupal.clueTipAttach);
}
