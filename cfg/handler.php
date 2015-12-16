<?php
	//error_reporting (0);
	
	//var_dump ($argv);
	$argc = count ($argv);
	
	switch ($argc)
	{
		case 4:
			echo 2 . PHP_EOL;
			echo 'my awesome main group' . PHP_EOL;
			echo 'my other main awesome group' . PHP_EOL;
			
			break;
		default:
			switch ($argv [1])
			{
				case 'user':
					echo 112233 . PHP_EOL;
					echo 'my awesome user name' . PHP_EOL;
					echo 'my awesome password' . PHP_EOL;
					echo 12321 . PHP_EOL;
					echo '/my/shell' . PHP_EOL;
					echo '/my/home/path' . PHP_EOL;
					echo 'my:gecos:field' . PHP_EOL;
					
					break;
				case 'password':
					echo 'my awesome encrypted password' . PHP_EOL;
					echo 1234567890 . PHP_EOL;
					echo 123 . PHP_EOL;
					echo 456 . PHP_EOL;
					echo 789 . PHP_EOL;
					echo 12345 . PHP_EOL;
					echo 67890 . PHP_EOL;
					
					break;
				case 'group':
					echo 1231231 . PHP_EOL;
					echo 'the awesome group name' . PHP_EOL;
					echo 'the awesome group password' . PHP_EOL;
					echo 3 . PHP_EOL;
					echo 'the awesome member' . PHP_EOL;
					echo 'the other awesome member' . PHP_EOL;
					echo 'the secret awesome member' . PHP_EOL;
			}
	}
	
