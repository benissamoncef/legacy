/*E Fichier : $Id: xzahc.h,v 1.9 2007/05/11 15:59:18 gesconf Exp $      Release : $Revision: 1.9 $        Date : $Date: 2007/05/11 15:59:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME _SS_SYSTEME_
------------------------------------------------------
* MODULE XZAH * FICHIER xzahc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Animation ihm
*
------------------------------------------------------
* HISTORIQUE :
* B.G.  18/10/94	: Creation
* B.G.  25/10/94        : Modif valeur XZAHC_VoieInconnue (1.2)
* C.T.  04/12/94        : Ajout XZAHC_VOIES_xxxx  et
*                         XZAHC_Bouchon XZAHC_Basculement (1.3)
* C.T.  27/03/95        : Ajout XZAHC_Gare (1.4)
* C.T.  30/03/95        : Ajout des objets mobiles de type accident (1.5)
* C.T.  05/04/95        : Ajout des objets inervention autre (1.7)
* P.N.  08/01/96        : Ajout des constantes de voies (dem/1372) (1.8)
* JPL.	11/05/07	: Decalage constantes de types perturbations meteo (lie a DEM 643-647) (1.9)
------------------------------------------------------ */



/*A differentes voies */

#define XZAHC_VoieInconnue	100

/*A type d'extr≈mit≈ */

#define XZAHC_Debut		0
#define XZAHC_Fin		1
#define XZAHC_Ponctuel		2

/*A tete et queue de bouchon */

#define XZAHC_Tete		0
#define XZAHC_Queue		1

/*A types d'objets mobiles (attention , ne pas reprendre les m¡mes valeurs
 que pour XZAEC_FMC_xxx */

#define XZAHC_Travaux_Mob_P	255
#define XZAHC_Travaux_Mob_R	254
#define XZAHC_Travaux_Fix_P	253
#define XZAHC_Travaux_Fix_R	252
#define XZAHC_Gendarme		251
#define XZAHC_Pompier		250
#define XZAHC_Depanneur		249
#define XZAHC_PMV_Fourgon	248
#define XZAHC_Patrouille	247
#define XZAHC_PMV_Mobile	246
#define XZAHC_Gare		245
#define XZAHC_Travaux_Mob_Tete_P	244
#define XZAHC_Travaux_Mob_Queue_P	243
#define XZAHC_Travaux_Mob_Tete_R	242
#define XZAHC_Travaux_Mob_Queue_R	241
#define XZAHC_Intervention		240
#define XZAHC_Meteo		200	/* les offsets sont XZAEC_MET_xxx */
#define XZAHC_Accident		64
#define XZAHC_Accident_VL	1	/* offset par rapport a accident */
#define XZAHC_Accident_PL	2	/* offset par rapport a accident */
#define XZAHC_Accident_Car	4	/* offset par rapport a accident */
#define XZAHC_Accident_Remorque	8	/* offset par rapport a accident */
#define XZAHC_Accident_Moto	16	/* offset par rapport a accident */

/*A type de donnees sur les voies initialisant le sous systeme EQUEXT */

#define XZAHC_VOIES_MODIF	1
#define XZAHC_VOIES_SUPP	2
#define XZAHC_VOIES_LUES	3

/*A Type d'evenement : bouchon ou basculement */

#define XZAHC_Basculement	0
#define XZAHC_Bouchon		1

/*A Type de voie */
#define XZAHC_BAU	0
#define XZAHC_VL	1
#define XZAHC_VM1	2	
#define XZAHC_VM2	3
#define XZAHC_VR	4

