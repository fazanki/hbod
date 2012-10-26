<ul class="facebook-feed">
<?php foreach ($items as $item): ?>
  <li class="item">
    <span class="facebook-note-picture"><img alt="<?php echo $feedid; ?>" src="http://graph.facebook.com/<?php echo $feedid; ?>/picture" /></span>
    <span class="facebook-note-title"><a rel="external" href="<?php echo $item->link['href']; ?>"><?php echo $item->title; ?></a></span>
    <span class="facebook-note-message"><?php echo $item->summary; ?></span>
    <span class="facebook-note-time"><?php echo t('!time ago.', array('!time' => format_interval(time() - strtotime($item->published)))); ?></span>
  </li>
<?php endforeach; ?>
</ul>
