<?php
for($i = 1;$i < 100;$i++)
{
	echo("declare " . rand() % 60000 . " " . $i . "\n");
}
for($i = 1;$i < 99;$i++)
{
	echo("connect " . (rand() % 120000 + 60000) . " " . $i . " " . $i . " " . ($i + 1) . " " . rand() % 100 . "\n");
	//echo("connect " . (rand() % 10000) + 10000 . " " . $i . " " . $i . " " . $i + 1 . "\n");
}
echo("end 600000\nstart\n");
