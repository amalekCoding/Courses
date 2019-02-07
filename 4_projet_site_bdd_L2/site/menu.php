<?php
session_start();
?>

<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <title>Menu</title>
        <link rel="stylesheet" href="style.css">
    </head>
    <body>
		<header>
			<div id="navbar" class="navbar">       
				<ul class="nav navbar-nav navbar-right">
				<?php if (isset($_SESSION['connecte']) && $_SESSION['connecte']==1): ?>      
					 <li><a href="deconnexion.php" class="btn  btn-danger">Deconnexion</a></li>                           
				 <?php else: ?>                          
					 <li><a href="login.php" class="">Se connecter</a></li>
					<li><a href="inscriptionn.php" class="">S'inscrire</a></li>                        
				<?php endif ?>
			  </ul>
			</div>
			
			<h1>Examen</h1>
			
		</header>

	<?php	
		if(isset($_SESSION['type'])){
			if($_SESSION['type']== 'surveillant'){
				$type = 'surveillant';
			}
				
			else if($_SESSION['type']== 'correcteur'){
				$type = 'correcteur';
			}	
			else{
				$type = 'candidat';

			}
			
			$menu = "menu_".$type.".php";
			
			$menu=preg_replace('/\s/', '', $menu); 					/*Permet d'enlever les espaces */
			include($menu);
		}
		
		else{
			include('menu1.php');					   
		 }

	
		
	?>


    </body>
</html>
