// $Id: README.txt,v 1.2 2009/12/08 01:09:49 aaron Exp $

Readme for Media: Archive

The Media: Archive project allows the embedded media field user to embed video
and audio hosted on Archive.org. Embedded Media Field module
(http://drupal.org/project/emfield) is a pre-requisite for this module.  To use
this module install both this module and Embedded Media Field. You'll also need
to install either Embedded Video Field and/or Embedded Audio Field as
appropriate (both are packaged with Embedded Media Field). Enable emfield, and
either emvideo or emaudio - then enable "Media: Archive".

After that, editors will be able to paste a URL or the embed code for an
Archive.org video or audio posting into the field, and it will be displayed
automatically. Additionally, that URL will be parsed automatically, so the
module will know the difference between an Archive.org posting and one from
another video or audio provider.

By default, for audio, this module uses the default embed code as used on the
Archive.org site which is currently a version of Flowplayer.  Users wishing to
use a different flash player have a number of choices both free/open source and
others.  Two free options are the Wordpress audio player (hey, it's open
source), and 1pixelout which I believe is currently public domain.  These
players would have to be downloaded separately.  You would also need to
override the theme function supplied by this module to change the default
output.  I will document this in this readme file or in the documentation at
Drupal.org soon.

Note: archive.org supports having multiple tracks associated with one item.  I
will be adding support for that shortly.  Right now it will just pick out the
first track it finds in the code you supply.
