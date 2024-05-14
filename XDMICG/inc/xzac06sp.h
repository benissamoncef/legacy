/*E*/
/*  Fichier : $Id: xzac06sp.h,v 1.1 1994/11/03 14:52:23 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac06.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	19/09/94	: Creation
* C.T.	23/09/94	: Enleve la mise en commentaire de ACRP06
*                         (V 1.3)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Action de configuration des voies d'un tunnel par sequence.
* 
* Sequence d'appel
* SP	XZAC06_Commande_Seq_Tunnel
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Machine	va_PosteOperateur_in
* XDY_Eqt		va_NumEqt_in
* XDY_Horodate	va_HeureLancement_in
* XDY_Booleen	va_Manuelle_in
* XDY_Octet	va_Priorite_in
* XDY_Entier	va_DistanceEvt_in
* XDY_Octet	va_Sequence_in
* 
* Arguments en sortie
* XDY_Entier	va_NumAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, equipement indisponible, 
*                 insertion impossible, pb commande
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* L'execution de l'actio est realisee dans le 
* cas de commande automatique par sequence
* Si l'actio a ete proposee par le plan d'actions, 
* positionner Priorite et DistanceEvt, sinon, les mettre a 0
* Si l'operateur pilote equipement par equipement, 
* positionner Manuelle a XDC_Vrai (l'operateur doit 
* quand meme renseigner la sequence a laquelle il compte aboutir)
* Fonction
* Verifier l'existence de l'evenement
* Verifier l'existence de l'equipement non indisponible 
* dans le district ou l'actio est executee.
* 
* Inserer dans la table ACTION et la table AFFICHAGE_TUNNEL.
* Traitement sur l'actio precedente sur l'equipement a commander :
* 	Chercher l'actio precedente :
* 	SELECT numero FROM ACTION 
* 	WHERE equipement, type, heure_lancement_fin = null, 
* 		heure_lancement = all
* 		(SELECT max(heure_lancement) FROM ACTION 
* 		WHERE equipement, type)
* 	
* 	Positionner heure_lancement_fin a HeureLancement 
* 	pour l'actio precedente
* 	Si l'actio precedente trouvee est liee a un autre 
* 	evenement alors l'autre evenement doit 	etre reveille 
* 	pour vol (appeler XZAR03).
* 
* Dans le cas de commande automatique par sequence :
* 	ACRP06 : Appeler XZET03 pour commander l'equipement
-------------------------------------------------*/

 
#define XZAC06_Commande_Seq_Tunnel	"XZAC;06" 
