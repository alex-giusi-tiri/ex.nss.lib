<?php
	//error_reporting (0);
	
	//var_dump ($argv);
	$argc = count ($argv);
	$valid = false;
	$valid_type = false;
	$document = new SimpleXMLElement ('<xml />');
	
	switch ($argc)
	{
		case 5:
			switch ($argv [3])
			{
				case 'id':
				case 'name':
					$getter_type = $argv [3];
					$valid_type = true;
			}
			
			if ($valid_type)
			{
				$getter = $argv [4];
				$get = $argv [1];
				
				switch ($get)
				{
					case 'user':
					case 'shadow':
						/*
						echo 112233 . PHP_EOL;					// User ID.
						echo 'my awesome user name' . PHP_EOL;	// User name.
						echo 'my awesome password' . PHP_EOL;	// User password.
						echo 12321 . PHP_EOL;					// Main group's ID.
						echo '/my/shell' . PHP_EOL;				// Shell.
						echo '/my/home/path' . PHP_EOL;			// Home.
						echo 'my:gecos:field' . PHP_EOL;		// GECOS.
						*/
						/*
						switch ($argv [2])
						{
							case 'get':
								$user = $document -> addChild ('user');
							
								$user -> addChild ('id', 112233);
								$user -> addChild ('name', 'my awesome user name');
								$user -> addChild ('password', 'my awesome user password');
								$groups = $user -> addChild ('groups');
									$groups_0 = $groups -> addChild (0);	// Main or 0.
										$groups_0 -> addChild ('id', 12321);
								$user -> addChild ('shell', 'my/shell');
								$user -> addChild ('home', 'my/home/path');
								$user -> addChild ('gecos', 'my:gecos:field');
							
								break;
							case 'set':
						}
						*/
						
						//$type = 'user';
						//$valid = true;
						
						//break;
					
					
					case 'password':
					case 'passwd':
						/*
						echo 'my encrypted password' . PHP_EOL;		// User's encrypted password.
						echo 1234567890 . PHP_EOL;					//
						echo 123 . PHP_EOL;
						echo 456 . PHP_EOL;
						echo 789 . PHP_EOL;
						echo 12345 . PHP_EOL;
						echo 67890 . PHP_EOL;
						*/
						/*
						switch ($argv [2])
						{
							case 'get':
								$user = $document -> addChild ('user');
							
								$user -> addChild ('id', 112233);
								$user -> addChild ('name', 'my awesome user name');
								$user -> addChild ('password', 'my awesome user password');
								$groups = $user -> addChild ('groups');
									$groups_main = $groups -> addChild ('main');
										$groups_main -> addChild ('id', 12321);
								$user -> addChild ('shell', 'my/shell');
								$user -> addChild ('home', 'my/home/path');
								$user -> addChild ('gecos', 'my:gecos:field');
							
								break;
							case 'set':
						}
						*/
						
						$type = $get;
						$valid = true;
						
						break;
					
					
					case 'groups':
						/*
						switch ($argv [2])
						{
							case 'get':
								$groups = $document -> addChild ('groups');
							
								// The member groups.
								$grps =
								array
								(
									1 => 'my awesome group',
									2 => 'my other awesome group'
								);
								$count = $groups -> addChild ('count', count ($grps));
							
								// The list of member groups.
								$members = $groups -> addChild ('members');
								foreach ($grps as $grp_id => $grp_name)
								{
									$members -> addChild ($grp_id, $grp_name);
								}
							
								//echo 2 . PHP_EOL;										// Member groups count.
								//echo 'my awesome secondary group' . PHP_EOL;			// List of members.
								//echo 'my other awesome secondary group' . PHP_EOL;
							
								break;
							case 'set':
						}
						*/
					
						$type = 'passwd';
						$valid = true;
						
						break;
					
					
					case 'group':
					case 'gshadow':
						/*
						echo 1231231 . PHP_EOL;							// Group ID.
						echo 'the awesome group name' . PHP_EOL;		// Group name.
						echo 'the awesome group password' . PHP_EOL;	// Group password.
						echo 3 . PHP_EOL;								// Count of user members.
																		// List of user members.
						
							echo 'the awesome member' . PHP_EOL;
							echo 'the other awesome member' . PHP_EOL;
							echo 'the secret awesome member' . PHP_EOL;
						*/
						
						$type = $get;
						$valid = true;
				}
			}
	}
	
	if ($valid)
	{
		header ('content-type: text/xml');
		
		switch ($type)
		{
			// Get details about a user,
			// including its member groups.
			case 'user':
			case 'password':
			case 'passwd':
			case 'shadow':
					// The member groups.
					$_members =
					array
					(
						// The main group should be included.
						12321 => 'my awesome main group name',
						111 => 'my awesome secondary group member',
						222 => 'my other awesome secondary group member',
						//333 => 'my awesome secondary group member',
						//444 => 'my awesome secondary group member',
						//555 => 'my awesome secondary group member',
						//666 => 'my awesome secondary group member',
						//777 => 'my awesome secondary group member',
						//888 => 'my awesome secondary group member',
						//999 => 'my awesome secondary group member',
						100 => 'my awesome secondary group member'
					);
					$count = count ($_members);
				
				$user = $document -> addChild ('user');
				
				$user -> addChild ('id', 112233);
				$user -> addChild ('name', 'my awesome user name');
				
				$authentication = $user -> addChild ('authentication');
					$password = $authentication -> addChild ('password');
						$password -> addChild ('cipher', 'none');			// 0 or "none".
						$password -> addChild ('value', '');
						
						if ($type == 'shadow')
						{
							$crypted = $password -> addChild ('crypted');
								$crypted -> addChild ('cipher', 'SHA512');
								$crypted -> addChild ('value', '$123ABC');
							$time = $password -> addChild ('time');
								$change = $time -> addChild ('change');
									$change -> addChild ('previous', '');
									$change -> addChild ('minimum', '');
									$change -> addChild ('maximum', '');
								$time -> addChild ('inactivation');
//								$expiration = $time -> addChild ('expiration');
//									$expiration -> addChild ('warning', '');
//									$expiration -> addChild ('value', '');
								$time -> addChild ('expiration', '');
								$time -> addChild ('expiration_warning', '');
						}
				
				$user -> addChild ('shell', '/my/shell');
				$user -> addChild ('home', '/my/home/path');
				$user -> addChild ('gecos', 'my,gecos,field');
				
					// The main group.
					$group = $user -> addChild ('group');	// Main or 0.
						$group -> addChild ('id', 12321);
						$group -> addChild ('name', 'the main group');
						//$group_authentication = $group -> addChild ('authentication');
						//	$group_password = $group_authentication -> addChild ('password');
						//		$group_password -> addChild ('cipher', 'none');			// 0 or "none".
						//		$group_password -> addChild ('value', '');
				
				// The list of member groups.
				// This list should exclude the main group member.
				$groups = $user -> addChild ('groups');
				$groups -> addChild ('count', $count);
				$groups_list = $groups -> addChild ('list');			// "list" or "all".
					foreach ($_members as $member_id => $member_name)
					{
						$member = $groups_list -> addChild ('member');
							$member -> addChild ('id', $member_id);
							$member -> addChild ('name', $member_name);
					}
					unset ($member);
				
				echo $document -> asXML ();
				
				break;
			
			
			// Get details about a group,
			// including its member users.
			case 'group':
			case 'gshadow':
					// The member users.
					$_members =
					array
					(
						11 => 'the awesome user member',
						22 => 'the other awesome user member'
					);
					$count = count ($_members);
				
				$group = $document -> addChild ('group');
				
				$group -> addChild ('id', 112233);
				$group -> addChild ('name', 'my awesome group name');
				
				$group_authentication = $group -> addChild ('authentication');
					$group_password = $group_authentication -> addChild ('password');
						$group_password -> addChild ('cipher', 'none');			// 0 or "none".
						$group_password -> addChild ('value', '');
				
				// The list of member users.
				$users = $group -> addChild ('users');
				$users -> addChild ('count', $count);
				$users_list = $users -> addChild ('list');			// "list" or "all".
					foreach ($_members as $member_id => $member_name)
					{
						$member = $users_list -> addChild ('member');
							$member -> addChild ('id', $member_id);
							$member -> addChild ('name', $member_name);
					}
					unset ($member);
				
				echo $document -> asXML ();
		}
	}
	

