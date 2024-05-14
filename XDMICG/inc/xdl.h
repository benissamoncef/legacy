/*E*/
/* Fichier : $Id: xdl.h,v 1.10 1996/08/22 13:39:21 gesconf Exp $	Release : $Revision: 1.10 $        Date : $Date: 1996/08/22 13:39:21 $
******************************************************
* STERIA * PROJET MIGRAZUR        FICHIER xdl.h
******************************************************
* HISTORIQUE
*
* T.Milleville  27 Oct 1994 	: Creation
*
* T.Milleville  13 Jan 1995 	: Ajout XDL_SUP59.. et 
*					XDL_SUP60..  V1.4
*
* T.Milleville  16 Jan 1995 	: Ajout XDL_SUP61.. et 
*					XDL_SUP62..	V1.5
*
*  T.MIlleville   25 Jan 95     : Ajout XDL_SUP63_SRVNOK V1.6
*  T.MIlleville   09 Mar 95     : Ajout XDL_SUP64_HORNOK V1.7
*  T.MIlleville   09 Aug 95     : Ajout XDL_SUP39SCHS V1.9
* Guilhou	22 aut 1996	: ajout EXTENSION OUEST V1.10
*****************************************************/
/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/

 
#ifndef ModuleXDL
#define ModuleXDL

#include <xdc.h>
#include <xdm.h>
#include <xdy.h>


/* ==================================================================== */
/* Ce fichier contient les libelles des messages et des alarmes     	*/
/* Chaque Sous-Systeme liste ses messages 				*/
/* ==================================================================== */
/* Chaque ligne doit respecter le format suivamt:			*/
/* <No> <Code Action> <Format>						*/
/* <dd> <AA>          "dwgyfgwfi %s %sjiwopfjio"			*/
/* ==================================================================== */
/* Les codes d'ations sont:						*/
/*		BS: 	Basculement du dserveur de donnees ou de 	*/
/*			communication 					*/
/* 		DL: 	Diffusion Locale a la machine emettrice     	*/
/* 		DS: 	Diffusion Site (aux machines du site de la 	*/
/*			machine emettrice 				*/
/* 		DX: 	Diffusion site et Administration (aux machines 	*/
/*			du site de la machine emettrice et Berne 	*/
/*		DG: 	Diffusion globale 				*/
/* 		DA: 	Diffusion au site d'administration (aux 	*/
/*			machines de Berne) 				*/
/*		AA:	Aucune Action					*/
/* ==================================================================== */

#define XDLC_MAXALRM	1000
/* redefinitions des OFFSET des sous-systemes inclus dans xdm.h */
/* OFFSET_SUP = 1000/10 
 * OFFSET_INT = 2000/10
 * ...
 */
#define OFFSUP	(OFFSET_SUP/10)
#define OFFGES	(OFFSET_IHM/10)
#define OFFCON	(OFFSET_GES/10)
#define OFFINT	(OFFSET_EXT/10)
#define OFFARC	(OFFSET_ARC/10)
#define OFFALG	(OFFSET_ALG/10)

/* Messages generales */
#define XDL_TEXT	0

/* Messages du Sous-Systeme SUPERV */
/* ==================================================================== */
#define XDL_SUP01_CMDLTAC	(OFFSUP+1)
#define XDL_SUP02_CMDATAC	(OFFSUP+2)
#define XDL_SUP03_CMDRBOO	(OFFSUP+3)
#define XDL_SUP04_CMDASSY	(OFFSUP+4)
#define XDL_SUP05_CMDLSSY	(OFFSUP+5)
#define XDL_SUP06_CMDAAPP	(OFFSUP+6)
#define XDL_SUP07_CMDLAPP	(OFFSUP+7)
#define XDL_SUP08_CMDAMAC	(OFFSUP+8)
#define XDL_SUP09_DISPTAC	(OFFSUP+9)
#define XDL_SUP10_RELATAC	(OFFSUP+10)
#define XDL_SUP11_CREAMAC	(OFFSUP+11)
#define XDL_SUP12_CREATAC	(OFFSUP+12)
#define XDL_SUP13_CREASSY	(OFFSUP+13)
#define XDL_SUP14_DESTMAC	(OFFSUP+14)
#define XDL_SUP15_DESTTAC	(OFFSUP+15)
#define XDL_SUP16_DESTSSY	(OFFSUP+16)
#define XDL_SUP17_PRISLOG	(OFFSUP+17)
#define XDL_SUP18		0
#define XDL_SUP19		0
#define XDL_SUP20_MACETAT	(OFFSUP+20)
#define XDL_SUP21_MACSSYS	(OFFSUP+21)
#define XDL_SUP22_MACTACE	(OFFSUP+22)
#define XDL_SUP23		0
#define XDL_SUP24		0
#define XDL_SUP25		0
#define XDL_SUP26_MACMODE	(OFFSUP+26)
#define XDL_SUP27_MACVISU	(OFFSUP+27)
#define XDL_SUP28_CREAENT	(OFFSUP+28)
#define XDL_SUP29_DESTENT	(OFFSUP+29)
#define XDL_SUP30_SUPERHS	(OFFSUP+30)
#define XDL_SUP31_COMSDHS	(OFFSUP+31)
#define XDL_SUP32_COMSIHS	(OFFSUP+32)
#define XDL_SUP33_COMCOHS	(OFFSUP+33)
#define XDL_SUP34_COMEXHS	(OFFSUP+34)
#define XDL_SUP35_BASCCOM	(OFFSUP+35)
#define XDL_SUP36_SVCOMRD	(OFFSUP+36)
#define XDL_SUP37_BASCUSD	(OFFSUP+37)
#define XDL_SUP38_SVDONRD	(OFFSUP+38)
#define XDL_SUP39_SCHS		(OFFSUP+39)
#define XDL_SUP40		0
#define XDL_SUP41		0
#define XDL_SUP42		0
#define XDL_SUP43		0
#define XDL_SUP44_SVIMPHS	(OFFSUP+44)
#define XDL_SUP45_PERIPHS	(OFFSUP+45)
#define XDL_SUP46_STATIHS	(OFFSUP+46)
#define XDL_SUP47_COMBEOK	(OFFSUP+47)
#define XDL_SUP48_COMEXOK	(OFFSUP+48)
#define XDL_SUP49		0
#define XDL_SUP50_COMSCOK	(OFFSUP+49)
#define XDL_SUP51_COMSDOK	(OFFSUP+50)
#define XDL_SUP52		0
#define XDL_SUP53		0
#define XDL_SUP54		0
#define XDL_SUP55		0
#define XDL_SUP56_IMPSIOK	(OFFSUP+56)
#define XDL_SUP57_PERIPOK	(OFFSUP+57)
#define XDL_SUP58_SECNOK	(OFFSUP+58)
#define XDL_SUP59_DSKNOK	(OFFSUP+59)
#define XDL_SUP60_ALLDSKNOK	(OFFSUP+60)
#define XDL_SUP61_DSKOK		(OFFSUP+61)
#define XDL_SUP62_SYNCOK	(OFFSUP+62)
#define XDL_SUP63_SRVNOK	(OFFSUP+63)
#define XDL_SUP64_HORNOK	(OFFSUP+64)


/* Messages du Sous-Systeme INTEXT */
/* ==================================================================== */
#define XDL_INT01_CONETAB	(OFFINT+1)
#define XDL_INT02_CONIMPO	(OFFINT+2)
#define XDL_INT03_CONRETA	(OFFINT+3)
#define XDL_INT04_CONINTE	(OFFINT+4)
#define XDL_INT05_CSTINTE	(OFFINT+5)
#define XDL_INT06_MSGINCO	(OFFINT+6)
#define XDL_INT07_GRPINCO	(OFFINT+7)
#define XDL_INT08_FICINTE	(OFFINT+8)
#define XDL_INT09_MSGINTE	(OFFINT+9)
#define XDL_INT10_ECRINTE	(OFFINT+10)
#define XDL_INT11_CHORMOD	(OFFINT+11)
#define XDL_INT12_OPPORHS	(OFFINT+12)
#define XDL_INT13_OPPOROK	(OFFINT+13)
#define XDL_INT14_LCPORHS	(OFFINT+14)
#define XDL_INT15_LCHEUHS	(OFFINT+15)
#define XDL_INT16_ETHEUHS	(OFFINT+16)
#define XDL_INT17_ETBATHS	(OFFINT+17)
#define XDL_INT18_CLPOROK	(OFFINT+18)
#define XDL_INT19_TIOUTEC	(OFFINT+19)


/* Messages du sous-systeme GESIHM */
/* ==================================================================== */
#define XDL_GES01_GE505OK		(OFFGES+1)
#define XDL_GES02_GE505HS		(OFFGES+2)
#define XDL_GES03_DATAERR		(OFFGES+3)
#define XDL_GES04_NUMINCI		(OFFGES+4)
#define XDL_GES05_XZCSERR		(OFFGES+5)
#define XDL_GES06_SQLCERR		(OFFGES+6)
#define XDL_GES07_UTINCONNU		(OFFGES+7)
#define XDL_GES08_PASSERRONE		(OFFGES+8)
#define XDL_GES09_LISTEVIDE		(OFFGES+9)
#define XDL_GES10_SAISECT		(OFFGES+10)
#define XDL_GES11_SAISPASS		(OFFGES+11)
#define XDL_GES12_BP			(OFFGES+12)
#define XDL_GES13_DP			(OFFGES+13)
#define XDL_GES14_EP			(OFFGES+14)
#define XDL_GES15_ABS			(OFFGES+15)
#define XDL_GES16_Rela			(OFFGES+16)
#define XDL_GES17_DABS			(OFFGES+17)
#define XDL_GES18_Pac			(OFFGES+18)
#define XDL_GES19_InfoG			(OFFGES+19)
#define XDL_GES20_Secteur		(OFFGES+20)
#define XDL_GES21_SsSecteur		(OFFGES+21)
#define XDL_GES22_Image			(OFFGES+22)
#define XDL_GES23_Occup			(OFFGES+23)
#define XDL_GES24_ConvBP		(OFFGES+24)
#define XDL_GES25_ConvEP		(OFFGES+25)
#define XDL_GES26_CatPrev		(OFFGES+26)
#define XDL_GES27_CatTrain		(OFFGES+27)
#define XDL_GES28_GenreTrain		(OFFGES+28)
#define XDL_GES29_Utilisateur		(OFFGES+29)
#define XDL_GES30_Classe		(OFFGES+30)
#define XDL_GES31_Fonction		(OFFGES+31)
#define XDL_GES32_Justif		(OFFGES+32)
#define XDL_GES33_Annuaire		(OFFGES+33)
#define XDL_GES34_MsgCode		(OFFGES+34)
#define XDL_GES35_SecRadio		(OFFGES+35)
#define XDL_GES36_TronRadio		(OFFGES+36)
#define XDL_GES37_ParamFen		(OFFGES+37)
#define XDL_GES38_SAINUMINC		(OFFGES+38)
#define XDL_GES39_LISTEIMPRIME		(OFFGES+39)
#define XDL_GES40_FINLISTETDA		(OFFGES+40)
#define XDL_GES41_TDA_DISPARU		(OFFGES+41)
#define XDL_GES42_DEJA_TDA		(OFFGES+42)
#define XDL_GES43_DATE_INV		(OFFGES+43)
#define XDL_GES44_BD_DISPARU		(OFFGES+44)
#define XDL_GES45_DEJA_BD		(OFFGES+45)
#define XDL_GES46_MAC_NON_EXIS		(OFFGES+46)
#define XDL_GES47_SAIS_TOUS_CHAMPS	(OFFGES+47)
#define XDL_GES48_BASC_SD_IMPOSS	(OFFGES+48)
#define XDL_GES49_BASC_SC_IMPOSS	(OFFGES+49)
#define XDL_GES50_ARR_SSYS_IMPOSS	(OFFGES+50)
#define XDL_GES51_LAN_SSYS_IMPOSS	(OFFGES+51)
#define XDL_GES52_ARR_MIGRAZUR_IMPOSS	(OFFGES+52)
#define XDL_GES53_LAN_MIGRAZUR_IMPOSS	(OFFGES+53)
#define XDL_GES54_ARRMAC1_IMPOSS	(OFFGES+54)
#define XDL_GES55_ARRMAC2_IMPOSS	(OFFGES+55)
#define XDL_GES56_FONC_NON_EXEC		(OFFGES+56)
#define XDL_GES57_FONC_PAS_IMPL		(OFFGES+57)
#define XDL_GES58_COMM_TRAC_IMPOSS	(OFFGES+58)
#define XDL_GES59_PERIODE_VALIDITE	(OFFGES+59)
#define XDL_GES60_MESURE_NON_EXEC	(OFFGES+60)
#define XDL_GES61_AUCUN_TRAIN_TROUVE	(OFFGES+61)
#define XDL_GES62_CHAMP_OBLIG		(OFFGES+62)
#define XDL_GES63_INTERV_FAUX		(OFFGES+63)
#define XDL_GES63_Ralent_deja_existant  (OFFGES+64)
#define XDL_GES64_ACHEV			(OFFGES+65)

/* Messages du sous-systeme ARCHIV */
/* ==================================================================== */
#define XDL_ARC01_INSRBAN	(OFFARC+1)
#define XDL_ARC02_ERRABAN	(OFFARC+2)
#define XDL_ARC03_LANCARC	(OFFARC+3)
#define XDL_ARC04_RESSOCC	(OFFARC+4)
#define XDL_ARC05_DEMAARC	(OFFARC+5)
#define XDL_ARC06_BANDEHS	(OFFARC+6)
#define XDL_ARC07_BANDCAP	(OFFARC+7)
#define XDL_ARC08_RESSOCC	(OFFARC+8)
#define XDL_ARC09_GRAFNAR	(OFFARC+9)
#define XDL_ARC10_GRAFHHS	(OFFARC+10)
#define XDL_ARC11_RECONHS	(OFFARC+11)
#define XDL_ARC12_CREAGRA	(OFFARC+12)

  
/* Messages du sous-systeme CONFIG */
/* ==================================================================== */
#define XDL_CON01_SGBDAHS	(OFFCON+1)
#define XDL_CON02_SGBDPHS	(OFFCON+2)
#define XDL_CON03_TCIMP01	(OFFCON+3)
#define XDL_CON04_TCIMP02	(OFFCON+4)
#define XDL_CON05_TCIMP03	(OFFCON+5)
#define XDL_CON06_TCIMP04	(OFFCON+6)
#define XDL_CON07_SAUVBD_OK	(OFFCON+7)
#define XDL_CON08_PBTAR		(OFFCON+8)
#define XDL_CON09_PBAUTRE	(OFFCON+9)
#define XDL_CON20_TCIMP_DEB	(OFFCON+20)
#define XDL_CON21_TCIMP_OK	(OFFCON+21)
#define XDL_CON22_TCIMP_NOK	(OFFCON+22)
#define XDL_CON23_TCIMP_SQL	(OFFCON+23)
#define XDL_CON24_TCIMP_EXISTE	(OFFCON+24)
#define XDL_CON30_TCACT_DEB	(OFFCON+30)
#define XDL_CON31_TCACT_FIN	(OFFCON+31)
#define XDL_CON32_TCACT_NOK	(OFFCON+32)
#define XDL_CON33_TCACT_NDB	(OFFCON+33)
#define XDL_CON34_TCACT_SQL	(OFFCON+34)
#define XDL_CON35_TCACT_EXISTE	(OFFCON+35)
#define XDL_CON36_TCACT_TRFERR	(OFFCON+36)
#define XDL_CON37_TCACT_FICERR	(OFFCON+37)
#define XDL_CON38_TCACT_DAYERR	(OFFCON+38)
#define XDL_CON39_TCACT_CARERR	(OFFCON+39)
  
/* Messages du sous-systeme ALGORI */
/* ==================================================================== */
#define XDL_ALG01_Incorrect	(OFFALG+1)
#define XDL_ALG02_Incoherent	(OFFALG+2)
#define XDL_ALG03_Conversion    (OFFALG+3)
#define XDL_ALG04_Valable       (OFFALG+4)


/* Defnitions des types */
typedef enum
{
  XDLC_DAUCUN = 0,   
  XDLC_LOCAL,		/* DL */
  XDLC_SITEEXP,		/* DS */
  XDLC_GLOBAL, 		/* DG */
  XDLC_SITEADM,		/* DA */
  XDLC_SITE		/* DX */

} XDLT_IndDiff;

typedef enum
{
  XDLC_BAUCUN = 0,
  XDLC_BASC		/* BS */

} XDLT_IndBasc;

typedef char	XDLT_Format[512];
 	
typedef struct
{
  int 		Id;
  XDLT_IndDiff	IndDiff;
  XDLT_IndBasc	IndBasc;
  XDLT_Format	FormatFranc;
} XDLT_SystemeAlarme;

/* declaration de fonction externe */
extern int XDL_01SystemeAlarme (int 			va_NumTexte, 
				XDLT_SystemeAlarme*	pa_SystemeAlrm);

extern int XDL_02SystemeAlarme(char		*pa_texte,
		 	       int		va_numero_texte);
		 	       /*XDY_Langue	va_LangueCourante);*/

#endif
