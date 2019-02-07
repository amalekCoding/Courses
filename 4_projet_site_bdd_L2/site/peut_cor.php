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
			a { float : right ; padding:20px;}
			h2 { border:10px solid gray; text-align:center; padding:20px; width: 500px;  margin-left: 380px;}
			h3 { border:10px solid gray; text-align:center; padding:20px; width: 300px;  margin-left: 450px;}

			.bouton
			{
				color:#0000ff;
				font-size:24px;
				cursor:pointer;
				float : center;
			}
			.bouton:hover
			{
				text-decoration:underline;
			}
		</style>
		
		<script type="text/javascript" src="aff_cacher_tab.js"></script>
    </head>
    
    <body>
    
		<h2>Mes competances : </h2>



		<section>
			<table>
				<tr>
					<th colspan="5">Mes Module</th>
				</tr>
				<tr>			
				  <?php
				  
					$numpersonne = $_SESSION['numpersonne'];				  

					/* <!--
				-----	Affiche les modules que le correcteur peut corriger	------	    
					--> */
					$resultats = $cnx->query('SELECT libelle FROM peut_corriger p, modulee m WHERE idcorrecteur ='.$numpersonne.' and m.idmodule=p.idmodule');

					while($ligne = $resultats->fetch()){ 
						echo '<tr><td>' .$ligne['libelle'].'</td></tr>';

					}
			      ?>
				</tr>
			</table>
			

			<h3><span class="bouton" id="bouton_text" onclick="javascript:afficher_cacher('text');">Ajouter un module</span></h3>

		</section>
		
		
		<table id='text'>

			  <tr>
				<th>Module</th><th colspan="2">Valider</th>
			  </tr>
			  <tr>

				<form method= "GET" action="refr.php">
				 <td>
					 <!--
				-----Affiche les modules restants qui ne sont pas connues dans le tableau deroulant	------	    
					-->
				    <select size ="1" name="Tabmod">

							<?php	
							$modul = $cnx->query('SELECT * FROM modulee m WHERE NOT EXISTS (SELECT * FROM peut_corriger pc WHERE idcorrecteur='.$numpersonne.' AND EXISTS(SELECT * FROM modulee m2 WHERE m.idmodule = m2.idmodule and m.idmodule = pc.idmodule));');

							while($ligne = $modul->fetch()){ 
								echo ' <option value='.$ligne['idmodule'].'>'.$ligne['libelle'].'</option>';

							}
							?>
					
				    </select>
				 </td>
				<td><input type="submit" name="ajouter" value="Ajouter"/></td>
		      	    </FORM>
		      	    
		      	    
			  </tr>
		</table>
		
    	<?php
    	if(isset($_GET["Tabmod"])){

			$resultats = $cnx->query('INSERT INTO peut_corriger VALUES('.$numpersonne.','.$_GET["Tabmod"].')');
			echo $_GET["Tabmod"];
		}
		?>
							
							
    <script type="text/javascript">
    //<!--
        afficher_cacher('text');
    //-->
    </script>

		
		
		
		<?php
			include("footer.php");
		?>
		
	</body>
</html>





