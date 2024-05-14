/*E Fichier : $Id: cgcd_mSGBDeq.h,v 1.25 2021/05/10 08:52:13 pc2dpdy Exp $      Release : $Revision: 1.25 $        Date : $Date: 2021/05/10 08:52:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MVF * FICHIER MVF.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Fichier appartenant au module MVF contenant les fonctions
*  modificationSGBDequipements.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	19 Oct 1994	: Creation
* volcic	14 Nov 1994	: Modif XZAOT_ConfNAV => XZAOT_AjoutNAV	 (v1.3)
* torregrossa	01 Dec 1995	: Ajout cmvf_FctUti_MSE_XZAO191_190 (v1.4)
* MISMER	06 Nov 1996	: Ajout de la fonction de lecture des alarme et alerte v1.5 (DEM/1232)
* MISMER	10 Fev 1997	: Ajout des fonction temps de parcours v1.6 (DEM/1395)
* Guilhou	02 jan 1998	: ajout traitement zone d'influence PMV (dem/1534) 1.7
* torregrossa	21 Jan 1998	: Ajout cmvf_FctUti_MSE_XZAO421_420 (1545) 1.8
* guilhou	20 sep 2000	: modif interface cmvf_FctUti_MSE_XZAO417_416 1.9
* JMG		28/09/05 	: ajout PMVA BAF BAD 1.10
* AAZ       26/09/2007  : Ajout SIG ECL VEN ENR INC EXP SYS LTN DEM 666
* JMG	25/09/2009	: ajout cmvf_FctUti_MSE_XZAO352_351 1.13
* JPL	08/10/2010	: Ajout cmvf_FctUti_MSE_XZAO192_193 (DEM 948)  1.14
* JPL	15/02/2011	: Ajout parametre "distance" a cmvf_FctUti_MSE_XZAO428_ZON_PMVA (DEM 953)  1.15
* VR		15/12/11 : Ajout PRV (DEM/1016)
* JMG		01/03/17 : ajout  SAGA DEM1191
* LCL		26/06/17 : ajout zones urbaines (convergence)
* JPL		09/01/17 : Ajout cmvf_FctUti_MSE_XZAO261_262 (DEM 1266)  1.20
* LCL	06/02/2018 : Ajout cmvf_FctUti_MSE_XZAO258_CFE (CTRL_FEUX) 1.21 DEM1284
* LCL	28/05/18	: Ajout cmvf_FctUti_MSE_XZAO265_CAP (GABEB) 1.22 DEM1283
* JPL	24/01/19	: Inclusion des déclarations spécifiques à la configuration du SAE  1.23
* CGR   28/07/20	: Ajout cmvf_FctUti_MSE_XZAO492_IMU 1.24 DEM-SAE155
* CGR	06/10/20	: Ajout cmvf_FctUti_MSE_XZAO495_Zon_IMU et cmvf_FctUti_MSE_XZAO498_Msg_IMU 1.24 DEM-SAE155
* LCL	10/05/21	: Correction déclaration 495/498 1.25 DEM-SAE155
* ABK   18/08/23    : Ajout nouveau type Picto (DEM/483)
------------------------------------------------------ */

#ifndef MVF_SGBD_EQ
#define MVF_SGBD_EQ

/* fichiers inclus */
	/* includes systeme */
	
	#include        <stdio.h>

	/* includes Utilitaires */
	
		/* rtworks */
	#include        <rtworks/common.h>
	#include        <rtworks/ipc.h>
	#include        <rtworks/ipc_proto.h>
	
	/* includes applicatifs Migrazur */
	
	
	#include        "xdc.h"
	#include        "xdy.h"
	#include        "xdg.h"
	#include        "xzao_cfg.h"
	#include        "xzao.h"
	#include        "xzsc.h"
	#include        "xzsm.h"
	#include        "cgcd_mvf.h"
	#include	"xzao450.h"
	#include	"xzao451.h"
	#include 	"xzao720.h"
	#include 	"xzao730.h"
	#include 	"xzao257.h"
	#include	"xzao492.h"
	#include	"xzao495.h"
	#include	"xzao498.h"
    #include    "xzao2571.h"
/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int cmvf_FctUti_MSE_XZAO98_67(XZAOT_ConfRADT , 		XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO103_72( XZAOT_ConfPMV, 		XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO104_73(XZAOT_ConfMiniGTC, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO105_80(XZAOT_ConfDAI, 		XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO107_137(XZAOT_ConfSyVideo, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO112_142(XZAOT_ConfMAA, 		XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO113_143(XZAOT_ConfPIRAU, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO156_157(XZAOT_ConfTypNAV, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO165_166(XZAOT_ConfGTCNICE, 	XDY_Entier);
/* AAZ 14/11/2007 */
extern int cmvf_FctUti_MSE_XZAO0165_0166(XZAOT_ConfGTCNICERENOVEE, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO977_978  (XZAOT_ConfTubeRenove, 	    XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO979_980  (XZAOT_ConfDemiEchRenove,   XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO981_982  (XZAOT_ConfSeqAutSIG,       XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO891_892  (XZAOT_ConfSeqRetSIG,       XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO983_984  (XZAOT_ConfSeqAutECL,       XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO985_986  (XZAOT_ConfSeqAutVEN,       XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO987_988  (XZAOT_ConfSeqAutENR,       XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO989_990  (XZAOT_ConfSeqAutINC,       XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO1013_1014  (XZAOT_ConfSeqAutEXP,       XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO991_992  (XZAOT_ConfEqt, 	        XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO993_994  (XZAOT_ConfSeqSIG,          XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO995_996  (XZAOT_ConfSeqECL,          XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO997_998  (XZAOT_ConfSeqVEN,          XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO999_1000 (XZAOT_ConfSeqENR,          XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO1011_1012  (XZAOT_ConfSeqEXP,          XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO1001_1002(XZAOT_ConfSeqINC,          XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO1003_1004(XZAOT_ConfLitG,            XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO1005_1006(XZAOT_ConfLTNG,            XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO1007_1008(XZAOT_ConfSYSG,            XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO1009_1010(XZAOT_ConfEHTG,            XDY_Entier); 

/* AAZ */
extern int cmvf_FctUti_MSE_XZAO167_168(XZAOT_ConfSeqTube, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO173_174(XZAOT_ConfSeqEch, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO99_68(XZAOT_ConfChau, 		XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO106_81( XZAOT_ConfAnal, 		XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO108_138( XZAOT_ConfLTVideo , 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO110_140( XZAOT_ConfMoniteur, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO111_141( XZAOT_ConfMagneto, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO114_144( XZAOT_ConfPAU, 		XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO158_159( XZAOT_ConfScenCn, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO175_176( XZAOT_ConfDemiEch, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO177_178( XZAOT_ConfTube, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO100_69(  XZAOT_ConfPtMes, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO109_139( XZAOT_ConfCamera, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO192_193( XZAOT_ConfZoneCamera, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO160_161( XZAOT_AjoutNAV, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO169_170(XZAOT_ConfSeqAutTun, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO171_172(XZAOT_ConfSeqAutEch, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO179_180(XZAOT_ConfCaptTun, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO181_182(XZAOT_ConfFAV, 		XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO183_184(XZAOT_ConfPnGTC, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO191_190(XZAOT_ConfFax,	 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO197_187(XZAOT_ConfEqtGenLCR,	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO407_408(XZAOT_ConfEqtUGTP,	XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO403_404(XZAOT_ConfEqtZDP, 	XDY_Entier); 
extern int cmvf_FctUti_MSE_XZAO421_420(XZAOT_AjoutPAL, 		XDY_Entier); 

extern int cmvf_FctUti_MSE_XZAO422_720(XZAOT_ConfPMVA, XDY_Entier) ;
extern int cmvf_FctUti_MSE_XZAO422_730(XZAOT_ConfEqtSAGA, XDY_Entier) ;
extern int cmvf_FctUti_MSE_XZAO257_PRV(XZAOT_ConfPRV, XDY_Entier) ;
extern int cmvf_FctUti_MSE_XZAO2571_PIC(XZAOT_ConfPIC, XDY_Entier) ;
extern int cmvf_FctUti_MSE_XZAO553_Zone_Regul(XZAOT_ConfZoneReg, XDY_Entier) ;
extern int cmvf_FctUti_MSE_XZAO450_BAF(XZAOT_AjoutBAF, XDY_Entier) ;
extern int cmvf_FctUti_MSE_XZAO451_BAD(XZAOT_AjoutBAD, XDY_Entier) ;

extern int cmvf_FctUti_MSE_XZAO352_351(    char         *va_autoroute_tun,
		    XDY_Sens     va_sens_tun,
		XDY_PR        va_pr_tun,
		    char        *va_autoroute_zone,
		XDY_Sens    va_sens_zone,
		    XDY_PR        va_debut_zone,
		XDY_PR        va_fin_zone,
		    XDY_Octet   va_type_zone,
		char   *va_groupe_tunnel,
		    XDY_PR      va_PRPSTT,
		XDY_Booleen    va_inhibition_zone,
		    XDY_Entier         va_Resultat_out);




/* AAZ 26/09/2007 */
extern int cmvf_FctUti_MSE_XZAO640_600( XZAOT_ConfSIG, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO641_601( XZAOT_ConfECL, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO642_602( XZAOT_ConfVEN, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO643_603( XZAOT_ConfENR, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO644_604( XZAOT_ConfINC, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO645_605( XZAOT_ConfEXP, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO646_606( XZAOT_ConfSYS, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO647_607( XZAOT_ConfLTN, 	XDY_Entier);
/* AAZ */
extern int cmvf_FctUti_MSE_XZAO417_416( char  		*va_autoroute_pmv,
                                        XDY_Sens        va_sens_pmv,
                                        XDY_PR          va_pr_pmv,
                                        char            *va_site_pmv,
                                        char		*va_autoroute_zone,
                                        XDY_Sens        va_sens_zone,
                                        XDY_PR          va_debut_zone,
                                        XDY_PR          va_fin_zone,
                                        char            *va_direction_zone,
                                        XDY_Booleen     va_inhibition_zone,
					XDY_PR          va_pr_1_pmv_amont,
					XDY_Sens        va_sens_1_pmv_amont,
					char            *va_autoroute_1_pmv_amont,
					int             va_distance_1_pmv_amont,
					XDY_PR          va_pr_1_pmv_amont_bis,
					XDY_Sens        va_sens_1_pmv_amont_bis,
					char            *va_autoroute_1_pmv_amont_bis,
					int             va_distance_1_pmv_amont_bis,
					XDY_PR          va_pr_1_pmv_amont_ter,
					XDY_Sens        va_sens_1_pmv_amont_ter,
					char            *va_autoroute_1_pmv_amont_ter,
					int             va_distance_1_pmv_amont_ter,
                                        XDY_Entier      va_Resultat_out);

extern int cmvf_FctUti_MSE_XZAO428_ZON_PMVA( char  		*va_autoroute_pmv,
                                        XDY_Sens        va_sens_pmv,
                                        XDY_PR          va_pr_pmv,
                                        char            *va_site_pmv,
                                        XDY_Entier      va_distance_pmv,
                                        char		*va_autoroute_zone,
                                        XDY_Sens        va_sens_zone,
                                        XDY_PR          va_debut_zone,
                                        XDY_PR          va_fin_zone,
                                        char            *va_direction_zone,
					XDY_Sens	va_type_zone,
                                        XDY_Booleen     va_inhibition_zone,
                                        XDY_Entier      va_Resultat_out);



extern int cmvf_FctUti_MSE_XZAO433_ZON_BAF( char  		*va_autoroute_baf,
                                        XDY_Sens        va_sens_baf,
                                        XDY_PR          va_pr_baf,
                                        char            *va_site_baf,
                                        char		*va_autoroute_zone,
                                        XDY_Sens        va_sens_zone,
                                        XDY_PR          va_debut_zone,
                                        XDY_PR          va_fin_zone,
                                        XDY_Booleen     va_inhibition_zone,
                                        XDY_Entier      va_Resultat_out);



extern int cmvf_FctUti_MSE_XZAO434_ZON_BAD( char  		*va_autoroute_bad,
                                        XDY_Sens        va_sens_bad,
                                        XDY_PR          va_pr_bad,
                                        char            *va_site_bad,
                                        char		*va_autoroute_zone,
                                        XDY_Sens        va_sens_zone,
                                        XDY_PR          va_debut_zone,
                                        XDY_PR          va_fin_zone,
                                        XDY_Booleen     va_inhibition_zone,
                                        XDY_Entier      va_Resultat_out);


int cmvf_FctUti_MSE_XZAO816(    char         *va_autoroute_pmv,
                    XDY_Sens     va_sens_pmv,
                    XDY_PR        va_pr_pmv,
                    char        *va_site_pmv,
                    char        *va_autoroute_zone,
                    XDY_Sens    va_sens_zone,
                    XDY_PR        va_debut_zone,
                    XDY_PR        va_fin_zone,
                    char        *va_direction_zone,
                    XDY_Booleen    va_inhibition_zone,
                    XDY_PR        va_pr_1_pmv_amont,
                    XDY_Sens    va_sens_1_pmv_amont,
                    char        *va_autoroute_1_pmv_amont,
                    int        va_distance_1_pmv_amont,
                    XDY_PR        va_pr_1_pmv_amont_bis,
                    XDY_Sens    va_sens_1_pmv_amont_bis,
                    char        *va_autoroute_1_pmv_amont_bis,
                    int        va_distance_1_pmv_amont_bis,
                    XDY_PR        va_pr_1_pmv_amont_ter,
                    XDY_Sens    va_sens_1_pmv_amont_ter,
                    char        *va_autoroute_1_pmv_amont_ter,
                    int        va_distance_1_pmv_amont_ter,
                    XDY_Entier         va_Resultat_out);


int cmvf_FctUti_MSE_XZAO830(    char         *va_autoroute_pmv,
                    XDY_Sens     va_sens_pmv,
                    XDY_PR        va_pr_pmv,
                    char        *va_site_pmv,
                    XDY_Entier   va_distance_pmv,
                    char        *va_autoroute_zone,
                    XDY_Sens    va_sens_zone,
                    XDY_PR        va_debut_zone,
                    XDY_PR        va_fin_zone,
                    char        *va_direction_zone,
                    XDY_Sens    va_type_zone,
                    XDY_Booleen    va_inhibition_zone,
                    XDY_Entier         va_Resultat_out);

int cmvf_FctUti_MSE_XZAO953_952( XZAOT_ConfZoneUrbaine,
                    XDY_Entier         va_Resultat_out);

extern int cmvf_FctUti_MSE_XZAO261_262 (XZAOT_Conf_Station_DIT,	XDY_Entier);

extern int cmvf_FctUti_MSE_XZAO258_CFE (XZAOT_ConfCFE, XDY_Entier);

extern int cmvf_FctUti_MSE_XZAO265_CAP (XZAOT_ConfCAP, XDY_Entier);

extern int cmvf_FctUti_MSE_XZAO492_IMU (XZAOT_ConfIMU, XDY_Entier);

extern int cmvf_FctUti_MSE_XZAO495_Zon_IMU (XZAOT_Conf_Zon_IMU, XDY_Entier);

extern int cmvf_FctUti_MSE_XZAO498_Msg_IMU (XZAOT_ConfMsgIMU, XDY_Entier);


#endif
