/*E*/
/*  Fichier : $Id: xzae40sp.h,v 1.2 2019/05/13 10:25:34 devgfi Exp $        $Revision: 1.2 $        $Date: 2019/05/13 10:25:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae40sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	29/08/94	: Creation
* C.T.	28/09/94	: Modif sur NomPtCar value a " " 
*                         si pas de point caracteristique
*                         de meme pour TypeAlerte
* C.T.  14/10/94	: Ajout des parametres de sortie
*                         de localisation de la cause,
*                         le site du commentaire et
*                         le critere de voie neutralisee
*                         a la place de bloquee dans cas
*                         travaux, basculement, action SIS
*                         V 1.6
* C.T.  20/10/94	: Modif commentaire entete procedure V 1.7
*
* JPL	04/04/19 : Procédure dégroupée
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs generiques d'une fiche main 
* courante a l'heure precisee
-------------------------------------------------------
* Sequence d'appel
* SP	XZAE40_Lire_Fiche_MC
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in : par defaut heure courante
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Entier	va_Numero_out
* XDY_Octet	va_Cle_out
* XDY_Entier	va_Site_out
* XDY_Octet	va_Type_out
* XDY_Horodate	va_DebutPrevu_out
* XDY_Horodate	va_Debut_out
* XDY_Horodate	va_FinPrevu_out
* XDY_Horodate	va_Fin_out
* XDY_Horodate	va_Validation_out
* XDY_Entier	va_NumCause_out
* XDY_Octet	va_CleCause_out
* XDY_Octet	va_TypeCause_out
* XDY_Alerte	va_NumAlerte_out
* XDY_Horodate	va_HoroAlerte_out
* XDY_Nom	va_TypeAlerte_out
* XDY_Nom	va_Origine_out
* XDY_Booleen	va_Confirme_out
* XDY_Autoroute	va_Autoroute_out
* XDY_PR	va_PR_out
* XDY_Sens	va_Sens_out
* XDY_Octet	va_PointCar_out
* XDY_Nom	va_NomPtCar_out
* XDY_Voie	va_VR_out
* XDY_Voie	va_VM2_out
* XDY_Voie	va_VM1_out
* XDY_Voie	va_VL_out
* XDY_Voie	va_BAU_out
* XDY_Voie	va_VR_I_out
* XDY_Voie	va_VM2_I_out
* XDY_Voie	va_VM1_I_out
* XDY_Voie	va_VL_I_out
* XDY_Voie	va_BAU_I_out
* XDY_Autoroute	va_AutorouteCause_out
* XDY_PR	va_PRCause_out
* XDY_Sens	va_SensCause_out
* XDY_Horodate	va_HoroCause_out
*
*
* XDY_Octet	va_NumCommentaire_out
* XDY_Horodate	va_HorodateCommentaire_out
* XDY_Horodate	va_HoroPerimeCommentaire_out
* XDY_Commentaire	va_TexteCommentaire_out
* XDY_Site	va_Site_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: fiche non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Penser a lire le complement fonction du type d'evenement
* En formation, preciser l'horodate
* Les informations concernant les commentaires sont retournes
* dans une liste
* 
* Fonction
* Select dans les tables EVENEMENT, EVENEMENT_HISTORIQUE, 
* ALERTE, COMMENTAIRES where numero evenement and horodate
----------------------------------------------------*/

 
#define XZAE40_Lire_Fiche_MC	"XZAE40" 
