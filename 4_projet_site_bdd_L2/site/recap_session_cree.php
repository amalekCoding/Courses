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
			h2 { border:10px solid gray; text-align:center; padding:20px; width: 500px;  margin-left: 30%;}
			h3 { border:10px solid gray; text-align:center; padding:20px; width: 300px;  margin-left: 35%;}

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
		
	 <script type="text/javascript">
			function VerificationDestinataire(formulaire) {
				var cpt = 0;
				while (cpt < 10) {
				   if (document.getElementById("id_personne"+cpt).options[0].selected) {
					alert('Veuillez sélectionner pour chaque epreuve une salle');
					return false;
				   }
				cpt++;

				} 
		   return true;
		  }
    </script>      
    
    
    
  </head>
    
    <body>
    
		<h2>Recapitulatif : </h2>
		
		
		
		<form method= "POST" action="insertion_salle.php">

		<section>
			<table>
				<tr>
					<th>Module</th><th>Date/Heure</th><th>Salle</th>
				</tr>
				<tr>			
				  <?php
				  
					$numpersonne = $_SESSION['numpersonne'];				  

					/* <!--
				-----	Affiche les modules que le correcteur peut corriger	------	    
					--> */
					$resultats = $cnx->query('SELECT * FROM epreuve ep, modulee m WHERE m.idmodule=ep.idmodule and idsession ='.$_GET['idsess'].' ORDER BY codeepreuve');
					$cpt = 0;
					while($ligne = $resultats->fetch()){ 
						echo '<tr><td>' .$ligne['libelle'].'</td><td>' .$ligne['dateheure'].'</td>';
					?>	
					
					
					<td>
		
						<?php	
						echo "<select  name = salle[] id=id_personne".$cpt.'>';
									
							$req = $cnx->query('SELECT s.idsalle, s.nomsalle, bat.nombatiment FROM salle s, batiment bat WHERE s.batiment=bat.idbatiment');

							echo '<option value='.NULL.'>Salle</option>';
							while($salle = $req->fetch()){
								echo '<option value='.$salle['idsalle'].'>'.$salle['nomsalle'].' '.$salle['nombatiment'].'</option>';
							}
							$cpt = $cpt + 1;
							}
						?>
						</select>
					</td>
						
						
				</tr>
		
			  
			</table>
			

			<h3><span class="bouton" id="bouton_text" onclick="return(VerificationDestinataire(this));">Valider</span></h3>

		</section>
		
	<?php
 			echo "<input name = 'session' value =".$_GET['idsess']." type= 'hidden'/>";
		?>

	  <script type="text/javascript">
    //<!--
        afficher_cacher('ajouter');
    //-->
    </script>

		
			<table id='boutton_valider'>


			 </td>	
							
				<td><input type="submit" name="ajouter" value="Ajouter" onclick="return(VerificationDestinataire(this))"/></td>

			</table>
		  
		  
		 </form>
		<?php
			include("footer.php");
		?>
		
	</body>
</html>





