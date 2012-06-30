<?php
class famfam_handler_field_og_member_count extends views_handler_field_numeric {
  function query() {
    $table = $this->query->ensure_table('og');
    $sql = og_list_users_sql(1, 0, NULL, TRUE);
    $sql = str_replace('%d', 'og.nid', $sql);
    $this->query->add_field('', "($sql)", 'famfam_member_count');
    $this->field_alias = 'famfam_member_count';
  }
  
  function render($values) {
    $nid = $values->nid;
    $txt = $values->member_count;
    if (og_is_group_member($nid)) {
      // Change this, because of a bug in (OG? Views?).
      //$value = og_is_picture() ? l($txt, "og/users/$nid/faces") : l($txt, "og/users/$nid");
      $value = l($txt, "og/users/$nid");
      return check_plain($this->options['prefix']) . $value . check_plain($this->options['suffix']);
    }
    else {
      return parent::render($values);
    }
  }
}