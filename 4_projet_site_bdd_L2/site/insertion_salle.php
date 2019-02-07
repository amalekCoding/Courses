<?php
session_start();

if (!isset($_SESSION['login']) || $_SESSION['type'] != 'correcteur'){									
	header('Location: index.php'); 								/* On autorise que les correcteurs */
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
			table {  padding: 10px; margin-left: 250px; float : left;
	  
					border-collapse:collapse; border:1px solid gray; }
			td,th { border:1px solid gray; text-align:center; }
						h3 { border:10px solid gray; text-align:center; padding:20px; width: 300px;  margin-left: 750px;}

			table
			{
				border-collapse : collapse;
				background-color : #EBD9C8;
			}

			th,td
			{
				border : 1px solid black;
				height : 40px;
				
				float : center;
			}

			th
			{
				background-color : #D48030;
			}

			.impair
			{
				background-color : #BEA58E;
			}

				fieldset {
				  margin-bottom: 15px;
				  padding: 10px;
				  margin-left: 80px;
				  width: 200px;
				  float : left;
				}
		</style>
		
		<script type="text/javascript" src="aff_cacher_tab.js"></script>
    </head>
    
    <body>

    
		
			
			
			<?php
			
			      
				$session = $_POST['session'];
				
				$codeep = $cnx->query("SELECT codeepreuve FROM epreuve WHERE idsession =".$session."  ORDER BY codeepreuve");
				
				if($codeep){
					
					$cpt = 0;
					
					while($ligne = $codeep->fetch()){ 					
						
						$req = $cnx->query('UPDATE epreuve SET salle ='.$_POST['salle'][$cpt].' WHERE codeepreuve= '.$ligne['codeepreuve']);
				
						$cpt = $cpt + 1;
							
					}
					
					echo '<h3>Session creé avec Succes</h3>';
				}
				else 
					echo "<script type='text/javascript'>document.location.replace('index.php');
						</script>";
			?>
					


		
		
		
		<?php
			include("footer.php");
		?>
		
	</body>
</html>






