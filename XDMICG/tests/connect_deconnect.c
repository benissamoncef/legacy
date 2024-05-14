/*X  Fichier : $Id: connect_deconnect.c,v 1.1 1994/08/24 14:16:49 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/08/24 14:16:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAG * FICHIER connect_deconnect.c
*               fait appel a xzag 
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	16 Aug 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "ctpublic.h"
#include "cstypes.h"
#include "asql_com.h"
#include "xdc.h"
#include "xdy.h"
/*#include "xdc.h"*/
/*#include "xzag.h"*/
/*#include "xzst.h"*/


static char *version = "$Id: connect_deconnect.c,v 1.1 1994/08/24 14:16:49 mercier Exp $ : XZAG" ;


/* definition de fonctions externes */
extern int XZAG02_Connexion_Base(char *);
extern int XZAO15_PMV_amont(
			XDY_Autoroute ,
			XDY_PR ,
			XDY_Sens ,
			XDY_Octet ,
			XDY_Eqt *,
			XDY_District *,
			XDY_Autoroute *,
			XDY_PR *,
			XDY_Sens *,
			XDY_Entier *);


/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  main()

/*
* ARGUMENTS EN ENTREE : Aucun
*
*
* CONDITION D'UTILISATION
*	SQL serveur des machines testes doivent etre lance
*	Sur ces serveurs doit exister l utilisateur MIGRAZUR
*	                        ayant un passwd     MIGRAZUR
*				une base            PRC
*
* FONCTION 
*    Ouvre une connexion a la base PRC sur le serveur de donnees du Site
*    Initialise la variable globale au Module XZAGV_Connexion 
*    a la connexion 
*    Sybase si tout s est bien passe sinon elle est mise a NULL.
*
------------------------------------------------------*/
{

	int retcode;
	int attente=20;

	printf("Connexion a SDCI1\n");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);

	/* Attente avant de se deconnecter : permet de verifier	*/
	/* Via isql -Usa -P -SSQL_CI avec la commande sp_who   	*/
	/* que l on s est bien connecte en tant que MIGRAZUR   	*/
	/* sur la machine locale ou on a lance le test		*/
	printf("Mise en sommeil de %d secondes afin de verifier la connexion avec isql\n",attente);

	sleep(attente);

	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
	printf("Mise en sommeil de %d secondes afin de verifier\n que la deconnexion sur le serveur a bien ete effectuee\n ceci s effectue avec isql\n",attente);
	sleep(attente);

	printf("Connexion a SDDN1\n");
	retcode = XZAG02_Connexion_Base( "SDDN1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);

	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
	printf("Essai de connection a un serveur erronne : SDDDDDN1\n");
	retcode = XZAG02_Connexion_Base( "SDDDDDN1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
	retcode = XZAG03_Deconnexion_Base();
        printf("XZAG03_Deconnexion_Base retourne %d pour connexion inexistante\n",retcode);

}
