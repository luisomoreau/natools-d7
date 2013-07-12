<?php
/**
 * @file
 *
 * Display timeline navigation by year and month.
 *
 * variables :
 * - items : article objects
 */
?>
<div class="modulestarter_article_list">
  <ul>
  <?php foreach($articles as $article) :?>
     <li><?php print strip_tags($article->title) ?></li>
  <?php endforeach ?>
  </ul>
</div>

