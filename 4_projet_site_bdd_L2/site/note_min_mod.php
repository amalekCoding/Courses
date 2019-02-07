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

<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
 

  <meta name="title" content="Calculatrice" />
  <meta name="description" content="Permet de calculer"/>
  <meta name="keywords" content="mots-clé1, mots-clé2, ..."/>
 

  <style type="text/css" media="screen">
  			body {
				background-color:#ffd;
				font-family:Verdana,Helvetica,Arial,sans-serif;
			}
			table { margin:10px auto; border-collapse:collapse; border:1px solid gray; }
			td,th{ border:1px solid gray; text-align:center; padding:20px; }
			h2 { border:10px solid gray; text-align:center; padding:20px; width: 500px;  margin-left: 420px;}
  </style>
<!--
 	  <script type='text/javascript'>

		function checkboxes(){
			
						<?php 
		
		
			?>
			 
		var inputElems = document.getElementsByTagName("input"),
        count = 0;
          
		for (var i=0; i<inputElems.length; i++) {       
			if (inputElems[i].type == "checkbox" && inputElems[i].checked == true) 
			{
				count++;

			}
			   
		}
		if(count == 2){
			return true; 
						 window.open('equerre_validation.html');

		} 
		else{
			alert("vous n\'avez pas coché assez de case");
			return false;
		}
			document.location.href="http://manouvellepage.com"; 
	}
	 
	</script> 
 -->
 </head>
 
 
	<body>
 


		<h2>Completer : </h2>
  
			<?php 
				echo $_GET['var'] ;
			?>
						
						
		<section>
			<table>
				<tr>
					<th colspan="5">Donner une Note mini</th>
				</tr>
				
				<tr>
					<form method= "POST" action="">
								
					<?php
						$resultats = $cnx->query("SELECT * FROM contient c JOIN modulee m ON m.idmodule=c.idmodule WHERE c.idsession=". $_GET['var']);

						while($ligne = $resultats->fetch()){
							echo '<tr>';
							echo '<td>' .$ligne['libelle'].'</td>';
					?>
					
					<td  colspan="5">
						<select name="Note[]">
						<?php
							echo '<option value='.NULL.'>Note</option>';
							for ($i =0; $i <= 20; $i++){
								echo '<option value="'.$i.'">'.$i.'</option>';
							}
						?>
						</select>
					</td>

				<?php	
				} 
				?>


<div id="boutonsidentite" class="boutonpp">
	<button type="button" value="valider2" id="valider2" onmouseup="validerModif2();" disabled><?php echo '<a href="mescorrections.php?idses='.$_GET['idses'].'&amp;idmod='.$_GET['id_module'].'">Valider</a>' ?> </button>

	<br/><br/>
				</tr>
			</table>
		</section>
						 		
			 		
 		<?php
			include("footer.php");
		?>
	</body>
</html>









