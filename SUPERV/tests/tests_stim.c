/*********************************************************
*	Fichier permettant de tester le bon fonctionnement
*	de la tache TIMER, ainsi que les fonctions 
*	d'interface XZSM_01DemanderProgrammation et 
*	XZSM_02AnnulerProgrammation 
*
***********************************************************/

#include <stdio.h>

#include "xdc.h"
#include "xzsm.h"
#include "xzst.h"
#include "xzsc.h"

#include <rtworks/common.h>
#include <rtworks/ipc.h>


	char *	vl_CleProg_test1 	= "TEST_1_TIMER";
	char *	vl_Dg_test1		= "TEST_1_TIMER";
	double 	offset_secondes_test1	= 5.0;
	int 	vl_Delai_test1		= 0;
        char *  vl_CleProg_test2        = "TEST_2_TIMER";
        char *  vl_Dg_test2             = "TEST_2_TIMER";
        double  offset_secondes_test2   = 5.0;
        int     vl_Delai_test2          = 5;
        char *  vl_CleProg_test3        = "TEST_3_TIMER";
        char *  vl_Dg_test3             = "TEST_3_TIMER";
        double  offset_secondes_test3   = 5.0;
        int     vl_Delai_test3          = 4;

	FILE * 	FIC_DIAL;


/********************
* TEST 1   permettant de declencher une demande 
*	de programmation sans delai 
*
***********************/
int test_1_timer()
{
	int 	vl_ValRet 		= XDC_OK;
	double	vl_Nb_Secondes = 0.0;


	/**** Demande de programmation **/


		/** Recherche du nombre de secondes ***/
		vl_Nb_Secondes = TutGetWallTime() + offset_secondes_test1;
		

		/*  Declenchement d'une programmation sur 
		*    un datagroup TEST_1_TIMER  avec le message 
		*  NUMERIC_DATA ayant comme cle de "TEST_1_TIMER"   *********/ 
	vl_ValRet = XZSM_01DemanderProgrammation(
		vl_Nb_Secondes,
		vl_Delai_test1,
		vl_Dg_test1,
		T_MT_NUMERIC_DATA,
		vl_CleProg_test1,
		XZSMC_SEC_VRAI );
		if (vl_ValRet != XDC_OK)
		{
			printf (" Probleme de programmation TIMER \n\n ");
		}

	return vl_ValRet;


}	/* fin test_1_timer */


/********************
* TEST 2   permettant de declencher une demande 
*	de programmation avec delai 
*
***********************/
int test_2_timer()
{
	int     vl_ValRet               = XDC_OK;
        double  vl_Nb_Secondes;


        /**** Demande de programmation **/


                /** Recherche du nombre de secondes ***/
                vl_Nb_Secondes = TutGetWallTime() + offset_secondes_test2;


                /*  Declenchement d'une programmation sur
                *    un datagroup TEST_2_TIMER  avec le message
                *  NUMERIC_DATA ayant comme cle de "TEST_2_TIMER"   *********/
        vl_ValRet = XZSM_01DemanderProgrammation(
                vl_Nb_Secondes,
                vl_Delai_test2,
                vl_Dg_test2,
                T_MT_NUMERIC_DATA,
                vl_CleProg_test2,
                XZSMC_SEC_FAUX );
                if (vl_ValRet != XDC_OK)
                {
                        fprintf (FIC_DIAL,
				" Probleme de programmation TIMER \n\n ");
                } 

                /** Recherche du nombre de secondes ***/
		vl_Nb_Secondes = TutGetWallTime() + offset_secondes_test3;


                /*  Declenchement d'une programmation sur
                *    un datagroup TEST_3_TIMER  avec le message
                *  NUMERIC_DATA ayant comme cle de "TEST_3_TIMER"   *********/
        vl_ValRet = XZSM_01DemanderProgrammation(
                vl_Nb_Secondes,
                vl_Delai_test3,
                vl_Dg_test3,
                T_MT_NUMERIC_DATA,
                vl_CleProg_test3,
                XZSMC_SEC_FAUX );
                if (vl_ValRet != XDC_OK)
                {
                        fprintf (FIC_DIAL,
				" Probleme de programmation TIMER 3 \n\n ");
                } 
	
	return vl_ValRet;


}	/* fin test_2_timer */



/******************************************
*  TEST  permettant d'annuler une programmation
*		d'un TIMER 
*
*****************************/
int test_3_timer()
{
	int 	vl_ValRet;



	/** Annulation du TIMER dont la cle est NULL et le 
	*  Datagroup est TEST_3_TIMER **/
	vl_ValRet = XZSM_02AnnulerProgrammation(
			XZSMC_CLEPROG_NUL,
			vl_Dg_test3, 0 );
	if (vl_ValRet != XDC_OK)
	{
		fprintf (FIC_DIAL," Probleme annulationdu TIMER !!\n");
	}
	
}	/* fin test_3_timer */

/******************************************
*  TEST  permettant d'attendre un message 
*
*****************************/
int test_4_timer()
{

	/* Attente du message de XDM_TST_TINIT ****/
	TipcSrvMainLoop(5.0);

}	/* fin test_4_timer */




/******************************************
*  TEST  permettant d'annuler une programmation
*		d'un TIMER 
*
*****************************/
int test_5_timer()
{
	int 	vl_ValRet;



	/** Annulation du TIMER dont la cle est NULL et le 
	*  Datagroup est TEST_2_TIMER **/
	vl_ValRet = XZSM_02AnnulerProgrammation(
			vl_CleProg_test2,
			vl_Dg_test2, 0 );
	if (vl_ValRet != XDC_OK)
	{
		fprintf (FIC_DIAL," Probleme annulationdu TIMER !!\n");
	}

}	/* fin test_5_timer */


main (int argc, char *argv[])
{

	int	return_val;
	int	vl_ValRet = XDC_OK;
	char *  Nom_Appli = "migrazur";
	char *  Nom_Tache = "tstim.x";
	XZSCT_NomMachine        NomMachine = "PODN4";
        XZSCT_NomTache  NomTache = "tstim.x";



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

	/*  Verification que la tache TIMER est bien connectee
	*		Pour cela verifier avec l'outil WATCH les 
	* abonnements au datagroup  NomMachine_tstim.x  et 
	* a NSUPTIME_NomMachine 
	*/
	

	/** attente pour continuer **/
	fprintf (FIC_DIAL, 
		" \n\nVerifier que la tache TIMER s'est bien connectee !!! \n");
	fprintf (FIC_DIAL,
	  " Pour continuer la suite du test , tapez sur la touche RETURN \n");
	return_val = getchar();

	/** Initialisation des Services du TIMER **/
	XZSM_00InitServices();

	/*****************************/
	/* Test 1 du timer */
	vl_ValRet = test_1_timer();

	/* Attente du message de XDM_TST_TSUP ****/
	TipcSrvMainLoop(7.0);


	/** attente pour continuer **/
	fprintf (FIC_DIAL, 
		" \n\nVerifier que le test 1 s'est bien deroule !!! \n");
	fprintf (FIC_DIAL,
	  " Pour continuer la suite du test , tapez sur la touche RETURN \n");
	return_val = getchar();
			

	/*****************************/
	/* Test 2 du timer */
	vl_ValRet = test_2_timer();

	/** attente pour continuer **/
	fprintf (FIC_DIAL, 
		" \n\nVerifier que le test 2 s'est bien deroule !!! \n");
	fprintf (FIC_DIAL,
	  " Pour continuer la suite du test , tapez sur la touche RETURN \n");
	return_val = getchar();


	/*****************************/
	/* Test 3 du Timer */
	vl_ValRet = test_3_timer(); 
	
	/** attente pour continuer **/
	fprintf (FIC_DIAL, 
		" \n\nVerifier que le test 3 s'est bien deroule !!! \n");
	fprintf (FIC_DIAL, 
		" Pour le Test 4 , Arreter la tache TIMER par Ctrl C \n\
		, puis la relancer en tapant tstim.x dans le \n \
		bon repertoire :: \n");
	fprintf (FIC_DIAL,
	  " Pour continuer la suite du test , tapez sur la touche RETURN \n");
	return_val = getchar();



	/*****************************/
	/* Test 4 du timer */
	vl_ValRet = test_4_timer();




	/** attente pour continuer **/
	fprintf (FIC_DIAL, 
		" \n\nVerifier que le test 4 s'est bien deroule !!! \n");
	fprintf (FIC_DIAL,
	  " Pour continuer la suite du test , tapez sur la touche RETURN \n");
	return_val = getchar();

	/*****************************/
	/* Test 5 du timer */
	vl_ValRet = test_5_timer();


	/** attente pour continuer **/
	fprintf (FIC_DIAL, 
		" \n\nVerifier que le test 5 s'est bien deroule !!! \n");
	fprintf (FIC_DIAL,
	  " Pour continuer la suite du test , tapez sur la touche RETURN \n");
	return_val = getchar();


	/* Fermeture du fichier de dialogue **/
	fclose(FIC_DIAL);

}	/* fin main */



