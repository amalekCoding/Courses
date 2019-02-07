<?php
include("menu.php");
session_start();
?>

<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <title>Connexion</title>


		<style type="text/css">
	

</style>
    </head>
    
    <body>
    
		<h2>FAQ</h2>
		<section>
			<ul>
				<li><a class="selected" href="#0">Questions</a></li>
				<li><a href="#0">Divers</a></li>

			</ul> 

		<div>
			<ul>
				<li class="cd-faq-title"><h2>Question</h2></li>
				<li>
					<a href="#0">Mot de passe oublié ?</a>
					<div>
						<p>Bouton mot de passe oublié et suiver les instruction</p>
					</div> 
				</li>

				<li>
					<a href="#0">Quand se déroulent les épreuves ?</a>
					<div class="cd-faq-content">
						<p>Lors de la session</p>
					</div> 
				</li>

				<li>
					<a href="#0">Où se passent les épreuves ?</a>
					<div>
						<p>Sauf exception, les épreuves écrites obligatoires se déroulent dans votre établissement d’origine.</p>
					</div> 
				</li>
			</ul> 

			<ul>
				<li class="cd-faq-title"><h2>Divers</h2></li>
				<li>
					<a href="#0">Comment lire sa convocation ?</a>
					<div>
						<p>La convocation recèle des informations importantes, pensez donc à la conserver, même après les épreuves et surtout à bien la lire. </p>
					</div> 
				</li>
			</ul> 

	
		</div> 
	<a href="#0">Close</a>
</section>
		
		<?php
			include("footer.php");
		?>	
	
	</body>
</html>
