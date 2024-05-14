/*********************************************************
*	Fichier permettant de tester le bon fonctionnement
*	de la tache PING, ainsi que les fonctions 
*	d'interface XZSG_00InitServices ,
*	XZSG_01SurveillerReseau et 
*	XZSG_02ArretSurveiller
*
***********************************************************/

#include <stdio.h>

#include "xdc.h"
#include "xzsm.h"
#include "xzst.h"
#include "xzsc.h"
#include "xzsg.h"

#include <rtworks/common.h>
#include <rtworks/ipc.h>



	FILE * 	FIC_DIAL;


/********************
* TEST 1   permettant de declencher une demande 
*	de surveillance reseau
*
***********************/
int test_1_ping()
{
	int 	vl_ValRet 		= XDC_OK;
	XZSCT_NomMachine        pl_NomMachine = "\0";

	/* Recuperation du Nom de la machine */
	XZSC_07NomMachine (pl_NomMachine);

	/* Demande de surveillance Reseau **/
	vl_ValRet = XZSG_01SurveillerReseau(pl_NomMachine);	
	if (vl_ValRet != XDC_OK)
	{
		printf (" Probleme surveillance reseau ");
	}

	return vl_ValRet;


}	/* fin test_1_ping */



/********************
* TEST 2   permettant dattendre un Message 
*
***********************/
int test_2_ping()
{

        /* Attente du message de XDM_TSG_INIT ****/
	TipcSrvMainLoop(5.0);

}	/* fin test_2_ping */



/********************
* TEST 3   permettant d'arreter la surveillance
*	du reseau 
*
***********************/
int test_3_ping()
{
	int 	vl_ValRet 		= XDC_OK;
	XZSCT_NomMachine        pl_NomMachine = "\0";

	/* Recuperation du Nom de la machine */
	XZSC_07NomMachine (pl_NomMachine);

	/* Demande de surveillance Reseau **/
	vl_ValRet = XZSG_02ArretSurveiller(pl_NomMachine);	
	if (vl_ValRet != XDC_OK)
	{
		printf (" Probleme d'arret surveillance reseau ");
	}


}	/* fin test_3_ping */




/************ Main **********/
main (int argc, char *argv[])
{

	int	return_val;
	int	vl_ValRet = XDC_OK;
	char *  Nom_Appli = "migrazur";
	char *  Nom_Tache = "tspng.x";
	XZSCT_NomMachine        NomMachine = "PODN4";



	/*  Recuperation de l'argument qui est le nom 
	*	du terminal ou l'on execute le programme de
	*   	test. Ceci afin de rediriger la sortie standart **/
	/* Ouverture du fichier de dialogue */

	if( (FIC_DIAL = fopen(argv[argc-1],"w")) == (FILE *) NULL )
	{
		exit(0);
	}	


	/****  Connexion au RTServer ***/
	XZSC_01InitCnxSup (Nom_Appli,Nom_Tache,
                        XZSCC_INTERDIRE,
                        XZSCC_INTERDIRE,
                        XZSCC_INTERDIRE,
                        NULL,"/dev/null");


	/*  Verification que la tache PING est bien connectee
	*		Pour cela verifier avec l'outil WATCH les 
	* abonnements au datagroup  NomMachine_tspng.x  et 
	* a NSUPPING_NomMachine 
	*/
	

	/** attente pour continuer **/
	fprintf (FIC_DIAL, 
		" \n\nVerifier que la tache PING s'est bien connectee !!! \n");
	fprintf (FIC_DIAL,
	  " Pour continuer la suite du test , tapez sur la touche RETURN \n");
	return_val = getchar();

	/** Initialisation des Services du PING **/
	XZSG_00InitServices();

	/*****************************/
	/* Test 1 du Ping */
	vl_ValRet = test_1_ping();

	/** attente pour continuer **/
	fprintf (FIC_DIAL,
	   " \n\nVerifier que le test 1 s'est bien deroule !!! \n");
       	fprintf (FIC_DIAL, 
		" Pour le Test 2 , Arreter la tache PING par Ctrl C \n\
		, puis la relancer en tapant tspng.x dans le \n \
		bon repertoire :: \n");
	fprintf (FIC_DIAL,
" Pour continuer la suite du test , tapez sur la touche RETURN \n");
	return_val = getchar();



	/*****************************/
	/* Test 2 du Ping */
	vl_ValRet = test_2_ping();

	/** attente pour continuer **/
	fprintf (FIC_DIAL, 
		" \n\nVerifier que le test 2 s'est bien deroule !!! \n");
	fprintf (FIC_DIAL,
	  " Pour continuer la suite du test , tapez sur la touche RETURN \n");
	return_val = getchar();


	/*****************************/
	/* Test 3 du Ping */
	vl_ValRet = test_3_ping(); 
	
	/** attente pour continuer **/
	fprintf (FIC_DIAL, 
		" \n\nVerifier que le test 3 s'est bien deroule !!! \n");
	fprintf (FIC_DIAL,
	  " Pour continuer la suite du test , tapez sur la touche RETURN \n");
	return_val = getchar();


	/* Fermeture du fichier de dialogue **/
	fclose(FIC_DIAL);

}	/* fin main */



