<?php
session_start();

if (!isset($_SESSION['login']) || $_SESSION['type'] != 'correcteur'){									
	header('Location: index.php'); 					/* On autorise que les correcteurs */
	die();
} 



include("menu.php");
include("connexion_bd.php");
?>

<!DOCTYPE html>
<html>
    <head>
		
        <meta charset="utf-8">
        <title>Choix Module</title>
        <link rel="stylesheet" href="style.css">

		<style type="text/css">
			body {
				background-color:#ffd;
				font-family:Verdana,Helvetica,Arial,sans-serif;
			}
			table { margin:10px auto; border-collapse:collapse; border:1px solid gray; }
			td,th { border:1px solid gray; text-align:center; padding:20px; }
						h2 { border:10px solid gray; text-align:center; padding:20px; width: 500px;  margin-left: 420px;}

		</style>
    </head>
    
    <body>
    
		<h2>Choix du Module a Supprimer</h2>



		<section>
			<table>
				<tr>
					<th colspan="5">Choix Module</th>
				</tr>
				<tr>
					<form method= "POST" action="">
								
							<?php
							$resultats = $cnx->query('SELECT idmodule, libelle FROM modulee m WHERE idmodule NOT IN (SELECT idmodule FROM module_supp)');

							while($ligne = $resultats->fetch()){ 
								echo '<tr><td><input type="checkbox" name="mod[]" value='.$ligne['idmodule'].'></td><td>' .$ligne['libelle'].'</td></tr>';

							}
							?>
						<td colspan="5"><input type="submit" name="supprimer" value="Supprimer"/></td><br/>

					</form>

				</tr>
			</table>
		</section>
	
		
		
		<?php
		if($_POST){
			foreach($_POST['mod'] as $key => $value){
				$req= $cnx->query('INSERT INTO module_supp VALUES('.$value.')'); 
				
			}
		}
		
		?>
			
		<?php		
			include("footer.php");
		?>
		
	</body>
</html>



