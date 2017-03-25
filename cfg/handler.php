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
		$database = new PDO ('sqlite:/etc/system.db', 'root', '');
		$database -> setAttribute (\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION);
		
		
		header ('content-type: text/xml');
		
		switch ($type)
		{
			// Get details about a user,
			// including its member groups.
			case 'user':
			case 'password':
			case 'passwd':
			case 'shadow':
				$statement_user = $database -> prepare ('SELECT `Name`, `ID`, `Password`, `Group` AS "Group: Name", (SELECT `ID` FROM `Groups` WHERE `Groups`.`Name` = `Accounts`.`Group`) "Group: ID", `Shell`, `Home`, COALESCE ((SELECT `Value` FROM `Accounts: Other` WHERE `Account` = ? AND `Entity` = ?), \'\') AS "GECOS" FROM `Accounts` WHERE `' . ($getter_type == 'id' ? 'ID' : 'Name') . '` = ?');
				$statement_groups = $database -> prepare ('SELECT `Group` AS "Name", (SELECT `ID` FROM `Groups` WHERE `Groups`.`Name` = `Groups\' Members`.`Group`) AS "ID" FROM `Groups\' Members` WHERE `Member` = ?');
				
				$statement_user -> execute (array ($getter, 'GECOS', $getter));
				$statement_groups -> execute (array ($getter));
				
				$results_user = $statement_user -> fetchAll (\PDO::FETCH_ASSOC);
				$results_groups = $statement_groups -> fetchAll (\PDO::FETCH_ASSOC);
				
				//var_dump ($results_user, $results_groups);
				
				if (! empty ($results_user) && $results_groups !== false)
				{
					// The member groups.
					/*$_members =
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
					*/
					$members_count = count ($results_groups);
					
					
					$id = $results_user [0] ['ID'];
					$name = $results_user [0] ['Name'];
					$group ['name'] = $results_user [0] ['Group: Name'];
					$group ['id'] = $results_user [0] ['Group: ID'];
					$shell = $results_user [0] ['Shell'];
					$home = $results_user [0] ['Home'];
					$gecos = $results_user [0] ['GECOS'];
					$password = $results_user [0] ['Password'];
					
					
					$user = $document -> addChild ('user');
					
					$user -> addChild ('id', $id);
					$user -> addChild ('name', $name);
				
					$authentication = $user -> addChild ('authentication');
						$password = $authentication -> addChild ('password');
							$password -> addChild ('cipher', 'none');			// 0 or "none".
							$password -> addChild ('value', $password);
						
							if ($type == 'shadow')
							{
								$crypted = $password -> addChild ('crypted');
									$crypted -> addChild ('cipher', 'none');
									$crypted -> addChild ('value', $password);
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
				
					$user -> addChild ('shell', $shell);
					$user -> addChild ('home', $home);
					$user -> addChild ('gecos', $gecos);
				
						// The main group.
						$_group = $user -> addChild ('group');	// Main or 0.
							$_group -> addChild ('id', $group ['id']);
							$_group -> addChild ('name', $group ['name']);
							//$group_authentication = $group -> addChild ('authentication');
							//	$group_password = $group_authentication -> addChild ('password');
							//		$group_password -> addChild ('cipher', 'none');			// 0 or "none".
							//		$group_password -> addChild ('value', '');
				
					// The list of member groups.
					// This list should exclude the main group member.
					$groups = $user -> addChild ('groups');
					$groups -> addChild ('count', $members_count);
					$groups_list = $groups -> addChild ('list');			// "list" or "all".
						foreach ($results_groups as $member)
						{
							$_member = $groups_list -> addChild ('member');
								$_member -> addChild ('id', $member ['ID']);
								$_member -> addChild ('name', $member ['Name']);
						}
						unset ($_member);
				
					echo $document -> asXML ();
				}
				
				break;
			
			
			// Get details about a group,
			// including its member users.
			case 'group':
			case 'gshadow':
				$statement_group = $database -> prepare ('SELECT `Name`, `ID`, \'\' AS "Password" FROM `Groups` WHERE `' . ($getter_type == 'id' ? 'ID' : 'Name') . '` = ?');
				$statement_users = $database -> prepare ('SELECT `Member` AS "Name", (SELECT `ID` FROM `Accounts` WHERE `Accounts`.`Name` = `Groups\' Members`.`Member`) AS "ID" FROM `Groups\' Members` WHERE `Group` = ?');
				
				$statement_group -> execute (array ($getter));
				$statement_users -> execute (array ($getter));
				
				$results_group = $statement_group -> fetchAll (\PDO::FETCH_ASSOC);
				$results_users = $statement_users -> fetchAll (\PDO::FETCH_ASSOC);
				
				
				if (! empty ($results_group) && $results_users !== false)
				{
					/*
					// The member users.
					$_members =
					array
					(
						11 => 'the awesome user member',
						22 => 'the other awesome user member'
					);
					*/
					$members_count = count ($results_users);
					
					$id = $results_group [0] ['ID'];
					$name = $results_group [0] ['Name'];
					$password = $results_group [0] ['Password'];
					
					
					$group = $document -> addChild ('group');
					
					$group -> addChild ('id', $id);
					$group -> addChild ('name', $name);
					
					$group_authentication = $group -> addChild ('authentication');
						$group_password = $group_authentication -> addChild ('password');
							$group_password -> addChild ('cipher', 'none');			// 0 or "none".
							$group_password -> addChild ('value', $password);
							
							if ($type == 'gshadow')
							{
								/*
								$crypted = $password -> addChild ('crypted');
									$crypted -> addChild ('cipher', 'none');
									$crypted -> addChild ('value', $password);
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
								*/
							}
						
					// The list of member users.
					$users = $group -> addChild ('users');
					$users -> addChild ('count', $members_count);
					$users_list = $users -> addChild ('list');			// "list" or "all".
						foreach ($results_users as $member)
						{
							$_member = $users_list -> addChild ('member');
								$_member -> addChild ('id', $member ['ID']);
								$_member -> addChild ('name', $member ['Name']);
						}
						unset ($_member);
					
					echo $document -> asXML ();
				}
		}
	}
	

