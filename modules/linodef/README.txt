// $Id: README.txt,v 1.1.2.2 2009/04/23 12:30:13 roidanton Exp $

Linodef - Link nodes & terms and embed fields & terms
-----------------------------------------------------
To install, place the entire Linodef folder into your modules directory.
Then go to Administer -> Site building -> Modules and enable the "Linodef" module.

Requirements
------------
  - Drupal 6
  - CCK - to embed field values
  - Textfield - to embed field values
  - Taxonomy - to embed terms
  - Advanced help - to access the help documentation (optional)

Usage
-----
Use it like any other filter:

1) Activation
Go to Administer -> Settings -> Input types and activate the Filter "Linodef" for
the input type of your choice.

2) Order
Go to the sort/order tab of the input type and move Linodef to the top so it has
the lowest weight. This step is important! Only that way e.g. the HTML filter is
able to remove undesired HTML tags from elements embedded by Linodef.

Now you can use the tags as described at the input type tips page: /filter/tips.

To get more information - e.g. about the provided Buttons API - use the included
help documentation (Advanced help module required) or refer to the official
documentation at http://drupal.org/project/linodef.