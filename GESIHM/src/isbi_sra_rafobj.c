/*E*/
/*Fichier :  $Id: isbi_sra_rafobj.c,v 1.7 2020/04/01 13:48:29 gesconf Exp $      Release : $Revision: 1.7 $        Date : $Date: 2020/04/01 13:48:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSRA * FICHIER isbi_sra_rafobj.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Creation et Mise a jour des Variables d'animations 
*   graphiques : Rafraichissement des equipements 
*
------------------------------------------------------
 * HISTORIQUE :
 *
 * Niepceron	12 Sep 1994	: Creation
 * VR		19 Dec 2011	: Ajout PRV (DEM/1016)
 *JMG		13/08/12 : suppression optimisation affichage PMV 
*  JMG           03/03/17 : saga DEM1191
 * LCL		12/04/17 : Correction probleme longueur de chaine dans MSRA19_Vue
* PNI		13/03/20 : MSRA17_raf_vag_EqtSigGTCRenovee : Modif specifiques pour toulon SAE-113
* JMG		10/06/22 : brique API tisse 1.8
* LCL 11/05/22 : MAGISTRA Ajout mise en cache
* LCL 16/06/22 : MAGISTRA 311 Affinage mise en cache dans la hashmap
* LCL 23/06/22 : MAGISTRA 311 Correction voies emprise bouchons/neutralisation
* LCL 12/07/22 : Correctif envoi messages au CI lorsque machine destinataire=VC ou DP et vm_NomSite=CI
* LCL 26/05/23 : Correctif comparaison cle hashmap et valeur precedente pour envoi message au front
* LCL 12/06/23 : Correction MSRA13_raf_vag_Pan_Alerte
* LCL 14/06/23 : Envoi chaines message vides
* LCL 24/10/23 : Evolutions PMV DEM-473
* GGY 24/11/23 : Ajout MSRA16_raf_vag_PIC (DEM483)
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>
#include "isbi_sra_rafobj.h"
#include "isbi_sop.h"
#include "isbi_sma.h"
#include "xzaac.h"
#include "../inc/uthash.h"

/* definitions de constantes */
#define CM_LIMITE_EMISSION_SOCKET 1024

#define CM_NB_ETAT 5
#define CM_DISPO 1
#define CM_INHIB 2
#define CM_HS    3
#define CM_INDIS 4
#define CM_MARG  5

#define CM_SEQ_ECH 1
#define CM_SEQ_TUN 2
#define CM_FIC_SEQ_ECH "../fichiers/pmv/ConfigSequenceEchangeur.cfg"
#define CM_FIC_SEQ_TUN "/produits/migrazur/appliSD/fichiers/pmv/ConfigSequenceTunnel.cfg"

#define CM_SEPAR ","

#define XZEUC_ETAT_REPOS 1
#define XZEUC_ETAT_APPEL 2
#define XZEUC_ETAT_COMM  3
#define XZEUC_ETAT_GARDE 4

#define XZERC_NIV_FLUIDE    1
#define XZERC_NIV_PRESATURE 2
#define XZERC_NIV_SATURE    3

#define CM_LONG_TEXT_PMV 15

#define CM_DAI_ALERTE 1
#define CM_DAI_PASALERTE 0

#define CM_DELTA ((float)(1.0/( 2.0 * CM_NB_PICTO)))

#define CM_DELTA_PANNEAU ((float)(1.0/( 2.0 * CM_NB_PANNEAU)))

#define CM_BUFF_SOCK 600

/* definition de macro locales */

/* declaration de variables locales */
static int 		vm_taille_emise_normal = 0;
static int 		vm_taille_emise_courbes = 0;
static char 		*version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 : isbi_sra_rafobj" ;
extern FILE 		*vg_tracefile;
extern XDY_Booleen 	vg_pas_tisse;
extern char* 		vg_destinataire;

char *vm_table_picto[CM_NB_PICTO+1] = { "",
		"XXXX",
		"DANG",
		"CA",
		"SATU",
		"CHAN",
		"SI",
		"L90",
		"L110",
		"IDPL",
		"IDTV",
		"L40",
		"IA",
		"AI",
		"IAA",
		"AAI",
		"IAAA",
		"AAAI",
		"I.A",
		"A.I",
		"ACCI",
		"NEIG",
		"VENT",
		"GLIS",
		"PL",
		"L70",
		"L50",
		"L130",
      "DANG", 
		"XA15b",
		"XA16",
		"CA",
		"VENT",
		"NEIG",
		"XA26",
      "XA3",
      "XA3a",
      "XA3b",
      "GLIS",
      "I.A",
		"X1aB13_55",
		"X1aB13_75",
		"X1aB13_190",
		"X1aB9f",
		"X1aB13_35",
		"X1aB8",
		"X1aAK14",
		"X1aAK30",
      "A.I",
		"X1b-AK30",
		"X1b-AK31",
		"X1b-AK14",
		"X1b-AK5",
		"X1bB13_35",
		"X1bA26",
		"X2a-B1",
		"X2aB13_35",
		"X2b-B1",
		"X2bAK14",
		"X2bB13_35",
      "R21a",
      "R21b",
      "R21cd",
      "R21cg",
		"XC4a-50",
		"XC4a-70",
		"XC4a-90",
		"XC4a-110",
		"XC4b-70",
		"XCE15a",
		"XCE22",
		"B0",
		"SI",
		"XB11-2,5",
		"XB12-4,3",
		"XB13-5,5",
		"L100",
      "L110",
		"L120",
      "L130",
		"L50",
      "L70",
		"L80",
      "L90",
		"XB17-150",
		"XB26",
      "IDTV",
		"XB31",
		"XB33-70",
		"XB34",
		"XB34a",
      "IDPL",
		"XB44",
		"XB8",
		"XB9f",
		"XB9i",
		"XAK22",
      "SATU",
      "ACCI",
		"XAK32",
      "CHAN",
      "AI",
      "IA",
      "AAI",
      "IAA",
		"AAAI",
		"IAAA",
		"XKD10b3g",
		"XKD10b3d",
      "XKD81",
      "XKD82",
		"XKD811",
		"XKD9B1355",
		"XKD9B1375",
		"XKD9B13190",
		"XKD9_2+1",
		"XKD9B9F",
		"XKD9B8",
		"XKD9_1+1",
		"XKD9B13_35",
		"XKD9_1+2",
      "XXXX",
		"TEST",
		"BLANC",
		"ROUGE",
		"XB18c",
		"XB17-100",
		"X1bB8",
		"PL2",
		"CS"
};

char *vm_table_picto_SAGA[CM_NB_PICTO+1] = { "",
		"0",
		"3",
		"5",
		"2",
		"9",
		"18",
		"L90",
		"L110",
		"20",
		"19",
		"L40",
		"16",
		"13",
		"15",
		"12",
		"14",
		"11",
		"6",
		"7",
		"1",
		"10",
		"8",
		"4",
		"17",
		"L70",
		"L50","L130"
};
char *vm_table_panneau[CM_NB_PANNEAU+1] = { "",
					  "panneau_1",
					  "panneau_2",
					  "panneau_3",
					  "panneau_4",
					  "panneau_5" };

/* Buffers pour les messages envoyés au synoptique Flah */
char vl_msg[CM_BUFF_SOCK];

/* ----------- JBE: Stop envoi doublons ---------- */
struct msg_struct {
	char key[50];             /* key (string is WITHIN the structure) */
	char value[500];
	UT_hash_handle hh;         /* makes this structure hashable */
};

struct msg_struct *messages = NULL;

void iterate_msg(XZSCT_NomMachine va_machine)
{
struct msg_struct *s;
XDY_Datagroup vl_dg;
char vl_message[500];
char vl_cmd[500];
int vl_compteur=0;
	
time_t vl_start, vl_end;
unsigned long vl_time_used;

   vl_start = time(NULL);
	XZST_03EcritureTrace(XZSTC_INFO, "################ Demande init synoptiques pour machine %s", va_machine);

	XDG_EncodeDG2(vl_dg, XDG_API, vm_NomSite);

	/* on annonce le demarrage de l'initialisation */
	strcpy(vl_message, "|0|1");
	TipcSrvMsgWrite(vl_dg,
		XDM_IdentMsg(XDM_API_SYNOS),
		XDM_FLG_SRVMSGWRITE,
		T_IPC_FT_STR, vl_message,
		T_IPC_FT_STR, va_machine,
		NULL);

	if (!vg_pas_tisse) 
	{
      for (s=messages; s!=NULL; s=s->hh.next,vl_compteur++)
      {
         sprintf(vl_message,"%s",s->value);
         XZST_03EcritureTrace(XZSTC_DEBUG1, "################ envoi message [%s]", vl_message);
         /*sprintf(vl_cmd,"echo \"Init n %d - %s\" >>/tmp/hashmap",vl_compteur,vl_message);
         XZST_03EcritureTrace(XZSTC_DEBUG1, "cmd Init :%s",vl_cmd);
         system(vl_cmd);*/
            TipcSrvMsgWrite(vl_dg,
               XDM_IdentMsg(XDM_API_SYNOS),
               XDM_FLG_SRVMSGWRITE,
               T_IPC_FT_STR, vl_message,
               T_IPC_FT_STR, va_machine,
               NULL);
      }  
   }

   vl_end = time(NULL);
   vl_time_used = (unsigned long) difftime( vl_end, vl_start );
   XZST_03EcritureTrace(XZSTC_INFO, "Fin init, Nombre de ligne dans la table : %d, temp d'execution = %dsecs",vl_compteur, vl_time_used);
   sprintf(vl_cmd,"echo \"`date` Nbr ligne:%d temp exec:%d\" >>~/traces/hashmap_init_perf",vl_compteur, vl_time_used);
   XZST_03EcritureTrace(XZSTC_DEBUG1, "cmd add :%s",vl_cmd);
   system(vl_cmd);


	/* on annonce la fin de l'initialisation */
	strcpy(vl_message, "|0|0");
	TipcSrvMsgWrite(vl_dg,
		XDM_IdentMsg(XDM_API_SYNOS),
		XDM_FLG_SRVMSGWRITE,
		T_IPC_FT_STR, vl_message,
		T_IPC_FT_STR, va_machine,
		NULL);
}

void add_msg(char *key, char *value)
{
	struct msg_struct *s;
	/*char vl_cmd[500]="";
   sprintf(vl_cmd,"echo \"Add Key:%s value:%s\" >>/tmp/hashmap",key,value);
   XZST_03EcritureTrace(XZSTC_DEBUG1, "cmd add :%s",vl_cmd);
   system(vl_cmd);*/
	s=(struct msg_struct*)malloc(sizeof(struct msg_struct));
	strncpy(s->key, key,500);
	strncpy(s->value, value,500);

	HASH_ADD_STR( messages, key, s );
}

void delete_msg(struct msg_struct *message)
{
	/*char vl_cmd[500]="";
   sprintf(vl_cmd,"echo \"Del :%s\" >>/tmp/hashmap",message);
   XZST_03EcritureTrace(XZSTC_DEBUG1, "cmd DEL :%s",vl_cmd);
   system(vl_cmd);*/
	HASH_DEL(messages, message);
	free(message);
}

int canWeSend(char *chaineMsg)
{
	XZST_03EcritureTrace(XZSTC_DEBUG1, "################# traitement du message ->%s<- ################", chaineMsg);
	char separateur[1] = "|";
	char rKey[500]="";
	char rValue[11000]="";
   char delKey[500]="";
   char chaineMSGKey[500]="";
   char vl_dummystr[500];
	int limit = 0;
	int cptLimit = 0;
	int cpt = 0;
	int cptKey = 0;
	int cptValue = 0;
 	int vl_numero;
   int vl_numfmc, vl_clefmc, vl_idportion, vl_raf=0, vl_typemobile, vl_pr, vl_sens, vl_sens2, vl_idradt, vl_voie, vl_coeftrans, vl_coefext, vl_niveau, vl_actnum, vl_actcle, vl_localisation, vl_dummy,vl_debut_fin,vl_numordre,vl_coef,vl_type;
   char vl_nomBouchon[100];
   char vl_nom_ech[100];
   char vl_nomobj[100];
	struct msg_struct *outMessage2del;

        /*recupere le numero de message*/
	sscanf(chaineMsg,"|%d|",&vl_numero);
   strcpy(chaineMSGKey,chaineMsg);//on recopie le message dans la variable pour la cle si on remap pas
   if (vl_numero == CMD_FLASH_ETAT_EQUIP)
   {
      limit = 4;// On veuxt les 3 premiers champs, et la limite c'est le nombre de separateur, le message commence par un separateur donc +1
   }
	else if (vl_numero == CMD_FLASH_BOUCHON_CHAUSSEE)
   /* MAGISTRA : Ajout d'une cle - on ne differencie pas le typeraf, seul le dernier reste dans la hashmap */
   /* va_no_fmc.numero , va_no_fmc.cle , va_idportion , va_typeraf, va_sens, va_coeftrans, va_coefext, va_niveau, vl_nom_bouch */
   {
	  limit = 6;
     sscanf(chaineMsg,"|%d|%d|%d|%d|%d|%d", &vl_numero, &vl_numfmc, &vl_clefmc, &vl_idportion, &vl_raf, &vl_sens);
     // On compose la cle sans le vl_typeraf
     sprintf(chaineMSGKey,"|%d|%d|%d|%d|%d", vl_numero, vl_numfmc, vl_clefmc, vl_idportion, vl_sens);
   }
	else if (vl_numero == CMD_FLASH_BOUCHON_VOIE)
   /* MAGISTRA : Ajout d'une cle - on ne differencie pas le typeraf, seul le dernier reste dans la hashmap */
   /* CMD_FLASH_BOUCHON_VOIE */
   /* Traitement particulier pour ne garder que le dernier vg_type_rad pour chaque voie */
   /* ce traitement se fait lors du test de la modification du contenu par rapport a la cle */
   /* va_no_fmc.numero , va_no_fmc.cle , va_idportion , va_typeraf, */
   /* va_no_fmc.numero , va_no_fmc.cle , va_idportion , va_typeraf, va_pr, va_sens, va_voie, va_coeftrans, va_coefext, va_niveau, vl_nom_bouch  */
   {
	  limit = 8;     
     // On extrait le message complet
     sscanf(chaineMsg,"|%d|%d|%d|%d|%d|%d|%d|%d", &vl_numero, &vl_numfmc, &vl_clefmc, &vl_idportion, &vl_raf, &vl_pr, &vl_sens, &vl_voie);
     // On compose la cle sans le vl_typeraf
     sprintf(chaineMSGKey,"|%d|%d|%d|%d|%d|%d|%d", vl_numero, vl_numfmc, vl_clefmc, vl_idportion, vl_pr, vl_sens, vl_voie);
   }
	else if (vl_numero == CMD_FLASH_VOIE_NEUTRALISEE)
   /* MAGISTRA : Ajout d'une cle - on ne differencie pas le typeraf, seul le dernier reste dans la hashmap */
   /* CMD_FLASH_VOIE_NEUTRALISEE */
   /* Traitement particulier pour ne garder que le dernier vg_type_rad pour chaque voie */
   /* ce traitement se fait lors du test de la modification du contenu par rapport a la cle */
   /* va_no_fmc.numero , va_no_fmc.cle , va_idportion , va_pr, va_sens, va_voie, va_typeraf, va_coeftrans, va_coefext, vl_nom_bouch */
	{

     limit = 7;
     sscanf(chaineMsg,"|%d|%d|%d|%d|%d|%d|%d|%d", &vl_numero, &vl_numfmc, &vl_clefmc, &vl_idportion, &vl_pr, &vl_sens, &vl_voie, &vl_raf);
     // On compose la cle sans le vl_typeraf
     sprintf(chaineMSGKey,"|%d|%d|%d|%d|%d|%d", vl_numero, vl_numfmc, vl_clefmc, vl_idportion, vl_sens, vl_voie);
   }
	else if (vl_numero == CMD_FLASH_RADT_INVERSEE)
   /* MAGISTRA : Ajout d'une cle - on ne differencie pas le typeraf, seul le dernier reste dans la hashmap */
   /* va_no_fmc.numero , va_no_fmc.cle , va_idportion , va_pr, va_sens, va_voie, va_typeraf, va_coeftrans, va_coefext, va_idradt, vl_sens, vl_nom_bouch */
   {
	  limit = 7;
     sscanf(chaineMsg,"|%d|%d|%d|%d|%d|%d|%d|%d", &vl_numero, &vl_numfmc, &vl_clefmc, &vl_idportion, &vl_pr, &vl_sens, &vl_voie, &vl_raf);
     // On compose la cle sans le vl_typeraf
     sprintf(chaineMSGKey,"|%d|%d|%d|%d|%d|%d", vl_numero, vl_numfmc, vl_clefmc, vl_idportion, vl_sens, vl_voie);
   }
	else if (vl_numero == CMD_FLASH_MOBILE_PORTION)
   /* MAGISTRA : on ne differencie plus le typeraf, seul le dernier reste dans la hashmap */
   /* va_no_fmc.numero , va_no_fmc.cle , va_no_act.numero , va_no_act.cle , va_idportion, va_typeraf, va_type_mobile, (int) va_sens, (int) va_voie, va_coeftrans, vl_nom_bouch */
	/*  limit = 6;*/
	{
     limit = 6;
     
     sscanf(chaineMsg,"|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d", &vl_numero, &vl_numfmc, &vl_clefmc, &vl_actnum, &vl_actcle, &vl_idportion, &vl_raf, &vl_typemobile, &vl_sens, &vl_voie);
     // On compose la cle sans le vl_typeraf
     sprintf(chaineMSGKey,"|%d|%d|%d|%d|%d", vl_numero, vl_numfmc, vl_clefmc, vl_actnum, vl_actcle);
   }
	else if (vl_numero == CMD_FLASH_MOBILE_ECHANGEUR)
   /* MAGISTRA : on ne differencie plus le typeraf, seul le dernier reste dans la hashmap */
   /* va_no_fmc.numero , va_no_fmc.cle , va_no_act.numero , va_no_act.cle , pa_nom_ech, va_typeraf, va_type_mobile, (int) va_sens, (int) va_localisation, vl_nom_obj */
	/*  limit = 6;*/
	{
      limit = 6;
      sscanf(chaineMsg,"|%d|%d|%d|%d|%d|%[^|]|%d", &vl_numero, &vl_numfmc, &vl_clefmc, &vl_actnum, &vl_actcle, vl_nom_ech ,&vl_raf);
      // On compose la cle sans le vl_typeraf
      sprintf(chaineMSGKey,"|%d|%d|%d|%d|%d", vl_numero, vl_numfmc, vl_clefmc, vl_actnum, vl_actcle);
      
   }
	else if (vl_numero == CMD_FLASH_AFFECT_VOIE)
   {
      limit = 7;
      sscanf(chaineMsg,"|%d|%d|%d|%d|%d|%d|%d|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%d",
      &vl_numero, &vl_numfmc, &vl_clefmc
      ,&vl_debut_fin,&vl_idportion,&vl_numordre,&vl_coef,&vl_dummy
      ,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy
      ,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy
      ,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy
      ,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy
      ,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy
      ,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy
      ,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy
      ,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy
      ,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy,&vl_dummy
      ,&vl_dummy,&vl_dummy,&vl_raf);
      // On compose la cle sans le vl_typeraf
      sprintf(chaineMSGKey,"|%d|%d|%d|%d|%d|%d", vl_numero, vl_numfmc, vl_clefmc, vl_debut_fin, vl_idportion, vl_numordre);
   }
/*	else if (vl_numero == 23)
	  limit = 5;*/
   else if (vl_numero == CMD_FLASH_ANIM_EQUIP)
   {
      limit = 5;
   }
	else 
   { 
      for( cpt = 0 ; cpt < strlen(chaineMsg)+1 ; cpt++)
      {
         if (chaineMsg[cpt] == separateur[0])
         {
            limit++;
         }
      }
	}
   
   //Si on a un typeraf = 3, on supprime le message de la table et on return 1 pour envoyer l'etat au synoptique
   if (vl_raf == 3)
   {
      HASH_FIND_STR( messages, chaineMSGKey, outMessage2del);
      //Si on a un typeraf = 3, on supprime le message de la table et on return 1 pour envoyer l'etat au synoptique
      if (outMessage2del != NULL)
      {
         /* Existe */
         delete_msg(outMessage2del);
         XZST_03EcritureTrace(XZSTC_DEBUG1, "################# COMMANDE TYPE:%d REF SUPPR DELETED KEY %s", vl_numero, chaineMSGKey );      
      }
         XZST_03EcritureTrace(XZSTC_MESSAGE, "# # # message ->%s<- demande de suppr --> envoye, pas enregistre dans la table", chaineMsg);
         return 1;   
   }
	for( cpt = 0 ; cpt < strlen(chaineMsg)+1 ; cpt++ )
	{
		//XZST_03EcritureTrace(XZSTC_WARNING, "- Cyle ->%d<- - - - - - - - - - - - - - - - ", cpt);
		if(cptLimit < limit )
		{
			rKey[cptKey] = chaineMSGKey[cpt];
			//XZST_03EcritureTrace(XZSTC_WARNING, "- - - Ecriture de la Key = Key ->%s<- ; cptKey ->%d<- ", rKey, cptKey);
			cptKey++;		
		}//else  On prend tout le message on ne le coupe plus en cle / contenu
		//{
			rValue[cptValue] = chaineMsg[cpt];
			//XZST_03EcritureTrace(XZSTC_WARNING, "- - - Ecriture de la Value = Value ->%s<- ; cptValue ->%d<- ", rValue, cptValue);
			cptValue++;
		//}

		if(chaineMsg[cpt] == separateur[0])
		{
			//XZST_03EcritureTrace(XZSTC_WARNING, "- - - Decouverte separateur ! limit ->%d<- ", limit);
			cptLimit++;
		}
	}	
	rKey[cptKey]= '\0';
	rValue[cptValue] = '\0';
	XZST_03EcritureTrace(XZSTC_DEBUG1, "# # # Key ->%s<- ", rKey);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "# # # Value ->%s<- ", rValue);
	struct msg_struct *outMessage;
	HASH_FIND_STR( messages, rKey, outMessage);
	if(outMessage == NULL)
	{
		XZST_03EcritureTrace(XZSTC_MESSAGE, "# # # message ->%s<- jamais enregistre = envoye", chaineMsg);
		/* Existe pas */
		add_msg(rKey, rValue);
		return 1;
	}else
	{
		/* Existe */
      if ((strncmp(rValue, outMessage->value, strlen(rValue)) == 0) && (strlen(rValue)==strlen(outMessage->value)))
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "# # # message ->%s<- identique au precedent = NON envoye(%s--%s)", 
								chaineMsg,rValue,outMessage->value);
			/* valeur n'a pas changé  */
			return 0;
		}else
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "# # # message ->%s<- different du precedent = envoye(%s--%s)", 
								chaineMsg,rValue,outMessage->value);
			/* valeur a changé*/
			delete_msg(outMessage);
			add_msg(rKey, rValue);
			return 1;
		}
	}
}
/* ----------------------------------------------- */

/* declaration de fonctions internes */
void MSRA01_raf_vag_PMV_Message(); /* Rafraichissement des vag d'un PMV message */
void MSRA02_raf_vag_PMV_Picto();
void MSRA03_raf_vag_TRA();
void MSRA03_raf_vag_DIV();
void MSRA03_raf_vag_NivTRA();
void MSRA04_raf_vag_NAV();
void MSRA05_raf_vag_DAI();
int MSRA05_raf_vag_DAI_alerte();
void MSRA06_raf_vag_PAU();
void MSRA07_raf_vag_CAMERA();
int MSRA08_raf_vag_LIT();
void MSRA10_raf_vag_FAV();
int MSRA11_raf_vag_Panneaux();
void MSRA13_raf_vag_Pan_Alerte();
void MSRA12_raf_vag_PMVA();
void MSRA15_raf_vag_PRV();
void MSRA15_raf_vag_SAGA();
void MSRA13_raf_vag_BAF();
void MSRA14_raf_vag_BAD();
int MSRA14_raf_vag_DIS();
void MSRA03_raf_RADT();
void MSRA16_raf_vag_PIC();
int MSRA_Calcul_tendance();
float MSRA_No_Picto();
float MSRA_No_Picto_SAGA();
float MSRA_No_Panneau();
extern void MSRA09_raf_STRADA();
void MSRA09_raf_vag_STRADA();
int send_message();
int MSRA18_Masque();
int MSRA18_Masque_Projeter();
int MSRA19_Vue();
int MSRA20_TdpMZ();

extern int vg_cli_sock_ToFlash;
extern int vg_cli_sock_Courbes;


/* GTC Renovee */
int MSRA17_raf_vag_EqtSigGTCRenovee();

/* definition de fonctions externes */
extern int   MSVU_Creer_thash_vag();
extern int   MSVU_Recherche_vag();
extern int   MSVU_Affecte_vag();

typedef struct { int Numero ; } XDY_Etat_FAV;
typedef struct { int Numero ; } XDY_Etat_Panneaux;

void MSRA13_raf_vag_BAF(
	  va_etat_baf,
          pa_result,
          va_machine)

      XZEAT_Etat_BAF va_etat_baf;
	int *pa_result;
   char *va_machine;
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA13_raf_vag_BAF ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   int vl_val_num_flash;
   int vl_val_num_action;

   XZST_03EcritureTrace(   XZSTC_INTERFACE, "IN  : MSRA13_raf_vag_BAF" );
   
   /*A  Construction du prefise des variables associees a la baf */
   sprintf( vl_pref_var , "baf_%04d_" , va_etat_baf.Etat.Numero );
   
   XZST_03EcritureTrace(   XZSTC_WARNING , "BAF %d %d %d %d %d",
			(int)va_etat_baf.Etat.Modules[0][0], (int)va_etat_baf.Etat.Modules[3][0],
			(int)va_etat_baf.Etat.Modules[2][0] , (int)va_etat_baf.Etat.Modules[5][0],(int)XZENC_INACTIF );
   if ( ( (int)va_etat_baf.Etat.Modules[0][0] != (int)XZENC_INACTIF ) || ( (int)va_etat_baf.Etat.Modules[3][0] != (int)XZENC_INACTIF ))
      vl_val_num_flash = 1;
   else
      vl_val_num_flash = 0;
     
      XZST_03EcritureTrace(   XZSTC_WARNING , "BAF %d", vl_val_num_flash);
   if ( ( (int)va_etat_baf.Etat.Modules[2][0] != (int)XZENC_INACTIF ) || ( (int)va_etat_baf.Etat.Modules[5][0] != (int)XZENC_INACTIF ))
      vl_val_num_action = 1;
   else
      vl_val_num_action = 0;
          
     
   sprintf( vl_nom_var , "%sflash" , vl_pref_var);

   sprintf( vl_nom_var , "%saction" , vl_pref_var);
   /*A Mise a jour de la vue */

sprintf(vl_msg, "|10|47|%d|flash|%d", va_etat_baf.Etat.Numero, vl_val_num_flash);

if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num_flash , vl_nom_var );
		*pa_result = XDC_NOK;
		return;
	}
}

 sprintf(vl_msg, "|10|47|%d|action|%d", va_etat_baf.Etat.Numero, vl_val_num_action);
if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num_action, vl_nom_var );
		*pa_result = XDC_NOK;
		return;
	}
}

XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA13_raf_vag_BAF" );
*pa_result = XDC_OK;
return;
}

void MSRA15_raf_vag_PRV(
		va_etat_prv,
		pa_result,
      va_machine)
XZEAT_Etat_PRV va_etat_prv;
int *pa_result;
char *va_machine;
{
	char vl_nom_var[256];
	char vl_pref_var[256];
	char vl_var[15];

	sprintf( vl_pref_var , "prv%04d_" , va_etat_prv.Numero);
	sprintf(vl_msg, "|10|62|%d|vitesse|%s",va_etat_prv.Numero,va_etat_prv.Vitesse);
   /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
   canWeSend(vl_msg);
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			*pa_result = XDC_NOK;
			return;
		}

	if (strlen(va_etat_prv.Bandeau)==0)
	  strcpy(vl_var," ");
	else
	  strcpy(vl_var,va_etat_prv.Bandeau);
	sprintf(vl_msg, "|10|62|%d|texte_picto|%s",va_etat_prv.Numero,vl_var);
   /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
   canWeSend(vl_msg);
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			*pa_result = XDC_NOK;
			return;
		}


}

void MSRA16_raf_vag_PIC (va_etat_pic, pa_result, va_machine)
						XZEAT_Etat_PIC	va_etat_pic;
						int				*pa_result;
						char			*va_machine;
{
	char vl_nom_var [256];
	char vl_pref_var [256];
	char vl_var [15];

	sprintf(vl_pref_var , "pic%04d_", va_etat_pic.Numero);

	if (strlen (va_etat_pic.Picto) == 0)
		strcpy (vl_var," ");
	else
		strcpy (vl_var, va_etat_pic.Picto);
	sprintf (vl_msg, "|10|69|%d|Picto|%s", va_etat_pic.Numero, vl_var);
	/* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
	canWeSend(vl_msg);
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		*pa_result = XDC_NOK;
		return;
	}

	if (strlen (va_etat_pic.Flash) == 0)
		strcpy (vl_var," ");
	else
		strcpy (vl_var, va_etat_pic.Flash);
	sprintf (vl_msg, "|10|69|%d|Flash|%s", va_etat_pic.Numero, vl_var);
	/* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
	canWeSend(vl_msg);
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		*pa_result = XDC_NOK;
		return;
	}

	if (strlen (va_etat_pic.Bandeau) == 0)
		strcpy (vl_var," ");
	else
		strcpy (vl_var, va_etat_pic.Bandeau);
	sprintf (vl_msg, "|10|69|%d|Bandeau|%s", va_etat_pic.Numero, vl_var);
	/* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
	canWeSend(vl_msg);
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		*pa_result = XDC_NOK;
		return;
	}
}

void MSRA15_raf_vag_SAGA(
	va_etat_saga,
	pa_result,
   va_machine)
XZEAT_Etat_Eqt_SAGA va_etat_saga;
int *pa_result;
char *va_machine;
{
  char vl_nom_var[256];
 char vl_pref_var[256];
  char vl_var[15];
  int vl_num_picto = 0;
  int vl_activer = 0;

  XZST_03EcritureTrace(   XZSTC_INTERFACE,"MSRA15_raf_vag_SAGA : eqt %d type %d",
  			va_etat_saga.Numero, va_etat_saga.TypeSAGA);
  if (va_etat_saga.Affichage1==XDC_PMV_HEURE) {
    strcpy(va_etat_saga.Texte1, "   HEURE   ");
    strcpy(va_etat_saga.Texte2,"");
    strcpy(va_etat_saga.Texte3,"");
    strcpy(va_etat_saga.Texte4,"");
    strcpy(va_etat_saga.Texte5,"");
  }

  switch(va_etat_saga.TypeSAGA) {
    case (XDC_SAGA_PMV_1_18):
      if (strlen(va_etat_saga.Texte1)> 0)
        vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte1|%s", va_etat_saga.TypeSAGA,
      				va_etat_saga.Numero,
				va_etat_saga.Texte1);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      sprintf(vl_msg, "|10|%d|%d|activation|%d", va_etat_saga.TypeSAGA,
      				va_etat_saga.Numero,
				vl_activer);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      break;


    case (XDC_SAGA_PMV_2_18) :
    case (XDC_SAGA_PMV_2_15):
      if (strlen(va_etat_saga.Texte1)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte1|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte1);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      if (strlen(va_etat_saga.Texte2)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte2|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte2);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      sprintf(vl_msg, "|10|%d|%d|activation|%d", va_etat_saga.TypeSAGA,
                                   va_etat_saga.Numero,
                                   vl_activer);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      break;

    case (XDC_SAGA_PMV_3_15):
      if (strlen(va_etat_saga.Texte1)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte1|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte1);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      if (strlen(va_etat_saga.Texte2)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte2|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte2);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      if (strlen(va_etat_saga.Texte3)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte3|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte3);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      sprintf(vl_msg, "|10|%d|%d|activation|%d", va_etat_saga.TypeSAGA,
                                   va_etat_saga.Numero,
                                   vl_activer);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      break;

    case (XDC_SAGA_PMV_2_18_P):
      if (strlen(va_etat_saga.Texte1)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte1|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte1);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      if (strlen(va_etat_saga.Texte2)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte2|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte2);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      sprintf(vl_msg, "|10|%d|%d|texte_picto|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Bandeau);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      if (strlen(va_etat_saga.Picto)> 0) vl_activer = 1;
      vl_num_picto = MSRA_No_Picto_SAGA( va_etat_saga.Picto);
      if (vl_num_picto==0) vl_num_picto++;
      sprintf(vl_msg, "|10|%d|%d|picto|%d", va_etat_saga.TypeSAGA,
      				va_etat_saga.Numero,
				vl_num_picto);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      sprintf(vl_msg, "|10|%d|%d|activation|%d", va_etat_saga.TypeSAGA,
                                   va_etat_saga.Numero,
                                   vl_activer);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      break;

    case (XDC_SAGA_PMV_3_15_P):
      if (strlen(va_etat_saga.Texte1)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte1|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte1);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      if (strlen(va_etat_saga.Texte2)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte2|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte2);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      if (strlen(va_etat_saga.Texte3)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte3|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte3);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      if (strlen(va_etat_saga.Bandeau)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte_picto|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Bandeau);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      vl_num_picto = MSRA_No_Picto_SAGA( va_etat_saga.Picto);
      if (vl_num_picto==0) vl_num_picto++;
      if (strlen(va_etat_saga.Picto)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|picto|%d", va_etat_saga.TypeSAGA,
      				va_etat_saga.Numero,
				vl_num_picto);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      sprintf(vl_msg, "|10|%d|%d|activation|%d", va_etat_saga.TypeSAGA,
                                   va_etat_saga.Numero,
                                   vl_activer);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      break;

    case (XDC_SAGA_PMVA):
      if (strlen(va_etat_saga.Texte1)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte1|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte1);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      if (strlen(va_etat_saga.Texte2)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte2|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte2);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      if (strlen(va_etat_saga.Texte3)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte3|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte3);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      if (strlen(va_etat_saga.Texte4)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte4|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte4);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      if (strlen(va_etat_saga.Texte5)> 0) vl_activer = 1;
      sprintf(vl_msg, "|10|%d|%d|texte5|%s", va_etat_saga.TypeSAGA,
                               va_etat_saga.Numero,
			       va_etat_saga.Texte5);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      sprintf(vl_msg, "|10|%d|%d|activation|%d", va_etat_saga.TypeSAGA,
                                   va_etat_saga.Numero,
                                   vl_activer);
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      send_message(vl_msg, CM_BUFF_SOCK, va_machine);
      break;

    case (XDC_SAGA_BAF):
	sprintf(vl_msg, "|10|%d|%d|flash|%d",
				va_etat_saga.TypeSAGA,
				va_etat_saga.Numero,
				va_etat_saga.Barriere);
   /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
   canWeSend(vl_msg);
	send_message(vl_msg, CM_BUFF_SOCK, va_machine);
	sprintf(vl_msg, "|10|%d|%d|action|%d",
				va_etat_saga.TypeSAGA,
				va_etat_saga.Numero,
				va_etat_saga.Barriere);
   /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
   canWeSend(vl_msg);
	send_message(vl_msg, CM_BUFF_SOCK, va_machine);
    	break;

    case (XDC_SAGA_BRA):
	sprintf(vl_msg, "|10|%d|%d|biseaux|%d",
				va_etat_saga.TypeSAGA,
				va_etat_saga.Numero,
				va_etat_saga.Barriere);
   /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
   canWeSend(vl_msg);
	send_message(vl_msg, CM_BUFF_SOCK, va_machine);
	sprintf(vl_msg, "|10|%d|%d|presignal|%d",
				va_etat_saga.TypeSAGA,
				va_etat_saga.Numero,
				va_etat_saga.Barriere);
   /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
   canWeSend(vl_msg);
	send_message(vl_msg, CM_BUFF_SOCK, va_machine);
        break;

    /*sav*/
    case (XDC_SAGA_SAV_1):
    case (XDC_SAGA_SAV_2):
    case (XDC_SAGA_SAV_3):
    case (XDC_SAGA_SAV_4):
    case (XDC_SAGA_SAV_5):
    case (XDC_SAGA_SAV_6):
        sprintf(vl_msg, "|10|%d|%d|etat|%d", va_etat_saga.TypeSAGA, va_etat_saga.Numero, va_etat_saga.Barriere);
    /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
    canWeSend(vl_msg);
	 send_message(vl_msg, CM_BUFF_SOCK, va_machine);
   	break;

    case (XDC_SAGA_DAI)   :
        break;

    case (XDC_SAGA_RADT):
        break;

    case (XDC_SAGA_RAU):
        break;

    case (XDC_SAGA_CAMERA):
       break;
  }
}
void MSRA14_raf_vag_BAD(
	  va_etat_bad,
          pa_result,
          va_machine)

      XZEAT_Etat_BAD va_etat_bad;
	int *pa_result;
   char *va_machine;
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA14_raf_vag_BAD ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   int vl_val_num_flash;
   int vl_val_num_action;

XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA14_raf_vag_BAD" );

/*A  Construction du prefise des variables associees a la baf */
sprintf( vl_pref_var , "bad_%04d_" , va_etat_bad.Etat.Numero );

XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSRA14_raf_vag_BAD: module 1 = %c",va_etat_bad.Etat.Modules[0][0]);
XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSRA14_raf_vag_BAD: module 2 = %c",va_etat_bad.Etat.Modules[1][0]);

if ( va_etat_bad.Etat.Modules[0][0] != XZENC_INACTIF )
	vl_val_num_flash = 1;
else
	vl_val_num_flash = 0;
      
   if ( va_etat_bad.Etat.Modules[1][0] != XZENC_INACTIF)
      vl_val_num_action = 1;
   else
      vl_val_num_action = 0;
      
     
   sprintf( vl_nom_var , "%sflash" , vl_pref_var);
   sprintf( vl_nom_var , "%saction" , vl_pref_var);
   /*A Mise a jour de la vue */

sprintf(vl_msg, "|10|48|%d|flash|%d", va_etat_bad.Etat.Numero, vl_val_num_flash);
if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num_flash , vl_nom_var );
		*pa_result = XDC_NOK;
		return;
	}
}
   sprintf(vl_msg, "|10|48|%d|action|%d", va_etat_bad.Etat.Numero, vl_val_num_action);
   if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num_action, vl_nom_var );
		*pa_result = XDC_NOK;
		return;
	}
}

XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA14_raf_vag_BAD" );
*pa_result = XDC_OK;
return;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un PMV message
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA01_raf_vag_PMV_Message(
          va_etat_pmv,
          pa_result,
          va_machine)
          
          XDY_Etat_PMV va_etat_pmv;
          int *pa_result;
          char *va_machine;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_pmv : Descriptif de l'etat du pmv a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA01_raf_vag_PMV_Message ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   float vl_val_num;
   char vl_activer=XDC_VRAI;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA01_raf_vag_PMV_Message" );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "pmv_%04d_" , va_etat_pmv.Numero );
   
   va_etat_pmv.Texte1[XDC_Lon_Ligne_PMV-1]='\000';
   va_etat_pmv.Texte2[XDC_Lon_Ligne_PMV-1]='\000';
   va_etat_pmv.Texte3[XDC_Lon_Ligne_PMV-1]='\000';
   va_etat_pmv.Texte4[XDC_Lon_Ligne_PMV-1]='\000';
   
   /*A Affectation de la variable pmx_xxxx_texte1 */
   sprintf( vl_nom_var , "%stexte1" , vl_pref_var);
   switch ( va_etat_pmv.Affichage1 )
      {
      case XDC_PMV_TEXTE :
      	 vl_activer=XDC_VRAI;
      	 sprintf(vl_msg, "|10|2|%d|texte1|%s", va_etat_pmv.Numero, va_etat_pmv.Texte1);
/*      	 if (canWeSend(vl_msg)) {*/
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , va_etat_pmv.Texte1, vl_nom_var );
			*pa_result = XDC_NOK;
			return;
		}
/*	}*/
         break;
      case XDC_PMV_TEMPERATURE :
      	 vl_activer=XDC_FAUX;
      	 sprintf(vl_msg, "|10|2|%d|texte1|  ", va_etat_pmv.Numero);
      	 if (canWeSend(vl_msg)) {
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte TEMPERATURE a la vag %s" , vl_nom_var );
			*pa_result = XDC_NOK;
			return;
		}
	}
         break;
      case XDC_PMV_HEURE :
      	 vl_activer=XDC_FAUX;
      	 sprintf(vl_msg, "|10|2|%d|texte1|  ", va_etat_pmv.Numero);
      /*	 if (canWeSend(vl_msg)) {*/
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte HEURE a la vag %s" , vl_nom_var );
			*pa_result = XDC_NOK;
			return;
		}
/*	}*/
	break;
default :
	XZST_03EcritureTrace(   XZSTC_WARNING , "Type d'affichage 1 PMV invalide  %d" ,  va_etat_pmv.Affichage1 );
	*pa_result = XDC_NOK;
	return;
}

/*A Affectation de la variable pmx_xxxx_texte2 */
sprintf( vl_nom_var , "%stexte2" , vl_pref_var);
switch ( va_etat_pmv.Affichage2 )
{
case XDC_PMV_TEXTE :
	vl_activer=XDC_VRAI;
	sprintf(vl_msg, "|10|2|%d|texte2|%s", va_etat_pmv.Numero, va_etat_pmv.Texte2);
	/*if (canWeSend(vl_msg)) {*/
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , va_etat_pmv.Texte2, vl_nom_var );
			*pa_result = XDC_NOK;
			return;
		}
	/*}*/
         break;
      case XDC_PMV_TEMPERATURE :
      	 vl_activer=XDC_FAUX;
      	 sprintf(vl_msg, "|10|2|%d|texte2|TEMPERATURE", va_etat_pmv.Numero);
      	 if (canWeSend(vl_msg)) {
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte TEMPERATURE a la vag %s" , vl_nom_var );
			*pa_result = XDC_NOK;
			return;
		}
	}
         break;
      case XDC_PMV_HEURE :
      	 vl_activer=XDC_FAUX;
      	 sprintf(vl_msg, "|10|2|%d|texte2|HEURE", va_etat_pmv.Numero);
      	 /*if (canWeSend(vl_msg)) {*/
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte HEURE a la vag %s" , vl_nom_var );
			*pa_result = XDC_NOK;
			return;
		}
	/*}*/
         break;
      default :
         XZST_03EcritureTrace(   XZSTC_WARNING , "Type d'affichage 2 PMV invalide  %d" ,  va_etat_pmv.Affichage2 );
         *pa_result = XDC_NOK;
            return;
         
      }

/*A Affectation de la variable pmx_xxxx_texte3 */
sprintf( vl_nom_var , "%stexte3" , vl_pref_var);
switch ( va_etat_pmv.Affichage3 )
{
case XDC_PMV_TEXTE :
	vl_activer=XDC_VRAI;
	sprintf(vl_msg, "|10|2|%d|texte3|%s", va_etat_pmv.Numero, va_etat_pmv.Texte3);
	/*if (canWeSend(vl_msg)) {*/
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , va_etat_pmv.Texte2, vl_nom_var );
			*pa_result = XDC_NOK;
			return;
		}
	/*}*/

	break;
case XDC_PMV_TEMPERATURE :
	vl_activer=XDC_FAUX;
	sprintf(vl_msg, "|10|2|%d|texte3| ", va_etat_pmv.Numero);
	if (canWeSend(vl_msg)) {
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte TEMPERATURE a la vag %s" , vl_nom_var );
			*pa_result = XDC_NOK;
			return;
		}
	}         break;
      
case XDC_PMV_HEURE :
	vl_activer=XDC_FAUX;
	sprintf(vl_msg, "|10|2|%d|texte3| ", va_etat_pmv.Numero);
	/*if (canWeSend(vl_msg)) {*/
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte HEURE a la vag %s" , vl_nom_var );
			*pa_result = XDC_NOK;
			return;
		}
	/*}*/
        break;
      default :
         XZST_03EcritureTrace(   XZSTC_WARNING , "Type d'affichage 3 PMV invalide  %d" ,  va_etat_pmv.Affichage3 );
         *pa_result = XDC_NOK;
      }

   /* PN : Inactivation PMV */
   if ( !strcmp(va_etat_pmv.Texte2,"HEURE") )
   	vl_activer = XDC_FAUX;
   	
   if (vl_activer == XDC_VRAI)
   	{
	XZST_03EcritureTrace(   XZSTC_DEBUG1 , "Activer %d",va_etat_pmv.Numero);
        sprintf(vl_msg, "|10|2|%d|activation|1", va_etat_pmv.Numero);
      	/*if (canWeSend(vl_msg)) {*/
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'activer le pmv %d" , va_etat_pmv.Numero );
			*pa_result = XDC_NOK;
			return;
		}
	/*}*/
   	/*MSOP06_activer_PMV(va_etat_pmv.Numero);*/
   	}
   else
   	{
	XZST_03EcritureTrace(   XZSTC_WARNING , "Inactiver %d",va_etat_pmv.Numero);
        sprintf(vl_msg, "|10|2|%d|activation|0", va_etat_pmv.Numero);
      	/*if (canWeSend(vl_msg)) {*/
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'inactiver le pmv %d" , va_etat_pmv.Numero );
			*pa_result = XDC_NOK;
			return;
		}
	/*}*/
	/*MSOP06_inactiver_PMV(va_etat_pmv.Numero);*/
}

   /*A Affectation de la variable pmx_xxxx_texte4 si elle existe */
   if ( va_etat_pmv.Affichage4 != XDC_PMV_NON_EXIST )
   {
     sprintf( vl_nom_var , "%stexte_dir" , vl_pref_var);
     switch ( va_etat_pmv.Affichage4 )
      {
      case XDC_PMV_TEXTE :
      	 vl_activer=XDC_VRAI;
      	 sprintf(vl_msg, "|10|2|%d|texte_dir|%s", va_etat_pmv.Numero, va_etat_pmv.Texte4);
      	 /*if (canWeSend(vl_msg)) {*/
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , va_etat_pmv.Texte4, vl_nom_var );
				*pa_result = XDC_NOK;
				return;
			}
		/*}*/
         break;
      case XDC_PMV_TEMPERATURE :
      	 vl_activer=XDC_FAUX;
      	 sprintf(vl_msg, "|10|2|%d|texte_dir| ", va_etat_pmv.Numero);
      	 if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte TEMPERATURE a la vag %s" , vl_nom_var );
				*pa_result = XDC_NOK;
				return;
			}
		}
         break;
      case XDC_PMV_HEURE :
      	 vl_activer=XDC_FAUX;
      	 sprintf(vl_msg, "|10|2|%d|texte_dir| ", va_etat_pmv.Numero);
      	 if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte HEURE a la vag %s" , vl_nom_var );
				*pa_result = XDC_NOK;
				return;
			}
		}
         break;
      default :
         XZST_03EcritureTrace(   XZSTC_WARNING , "Type d'affichage 4 PMV invalide  %d" ,  va_etat_pmv.Affichage4 );
         *pa_result = XDC_NOK;
      }
   }   

/* Affectation de la variable pmx_xxxx_cligno1 */
/*   sprintf( vl_nom_var , "%scligno1" , vl_pref_var);
   if ( va_etat_pmv.Clign1 != XDC_PMV_PASCLIGNO)
      vl_val_num = (float) 1;
   else
      vl_val_num = (float) 0;
   sprintf(vl_msg, "|10|2|%d|cligno1|%f", va_etat_pmv.Numero, vl_val_num);
   if (send_message(vl_msg, CM_BUFF_SOCK) == -1)
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte a la vag %s" , vl_nom_var );
      *pa_result = XDC_NOK;
      return;
      }*/

   /* Affectation de la variable pmx_xxxx_cligno2 */
 /*  sprintf( vl_nom_var , "%scligno2" , vl_pref_var);
   if ( va_etat_pmv.Clign2 != XDC_PMV_PASCLIGNO )
      vl_val_num = (float) 1;
   else
      vl_val_num = (float) 0;
   sprintf(vl_msg, "|10|2|%d|cligno2|%f", va_etat_pmv.Numero, vl_val_num);
   if (send_message(vl_msg, CM_BUFF_SOCK) == -1)
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte a la vag %s" , vl_nom_var );
 *pa_result = XDC_NOK;
      return;
      }
*/

   /* Affectation de la variable pmx_xxxx_flash */
   sprintf( vl_nom_var , "%sflash" , vl_pref_var);
   if ( va_etat_pmv.Flash == XDC_PMV_FLASH )
      vl_val_num = (float) 1;
   else
      vl_val_num = (float) 0;
   sprintf(vl_msg, "|10|2|%d|flash|%d", va_etat_pmv.Numero, vl_val_num);
   if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte a la vag %s" , vl_nom_var );
		*pa_result = XDC_NOK;
		return;
	}
}

   /* Affectation de la variable pmx_xxxx_alternat , s'il y a une ligne alterna alors il ya alternat*/
   sprintf( vl_nom_var , "%salternat" , vl_pref_var);
   if ( ( va_etat_pmv.Alternat1[0] != '\000' ) ||  
        ( va_etat_pmv.Alternat2[0] != '\000' ) ||  
        ( va_etat_pmv.Alternat3[0] != '\000' ) ||  
        ( va_etat_pmv.Alternat4[0] != '\000' ) )
      {
      vl_val_num = (float) 1;
      }
   else
      {
      vl_val_num = (float) 0;
      }
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA01_raf_vag_PMV_Message" );
   *pa_result = XDC_OK;
   
   return;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un PMV DATEX message
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA01_raf_vag_PMVDATEX_Message(
          pa_nom,
          pa_ligne1,
          pa_ligne2,
          pa_ligne3,
          pa_ligne4, 
          va_machine)
          
char *pa_nom;
char *pa_ligne1;
char *pa_ligne2;
char *pa_ligne3;
char *pa_ligne4;
char *va_machine;

/*
* ARGUMENTS EN ENTREE :
*    Descriptif de l'etat du pmv a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA01_raf_vag_PMVDATEX_Message ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   float vl_val_num;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA01_raf_vag_PMVDATEX_Message" );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "pmv_%s_" , pa_nom );
   
   if (strlen(pa_ligne1)>XDC_Lon_Ligne_PMV) pa_ligne1[XDC_Lon_Ligne_PMV-1]='\000';
   if (strlen(pa_ligne2)>XDC_Lon_Ligne_PMV) pa_ligne2[XDC_Lon_Ligne_PMV-1]='\000';
   if (strlen(pa_ligne3)>XDC_Lon_Ligne_PMV) pa_ligne3[XDC_Lon_Ligne_PMV-1]='\000';
   if (strlen(pa_ligne4)>XDC_Lon_Ligne_PMV) pa_ligne4[XDC_Lon_Ligne_PMV-1]='\000';
   
   /*A Affectation de la variable pmx_xxxx_texte1 */
   sprintf( vl_nom_var , "%stexte1" , vl_pref_var);
   sprintf(vl_msg, "|10|2|%s|texte1|%s", pa_nom, pa_ligne2);
   if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , pa_ligne2 , vl_nom_var );
		return;
	}
}

   /*A Affectation de la variable pmx_xxxx_texte2 */
   sprintf( vl_nom_var , "%stexte2" , vl_pref_var);
   sprintf(vl_msg, "|10|2|%s|texte2|%s", pa_nom, pa_ligne3);
   if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , pa_ligne3 , vl_nom_var );
		return;
	}
}

   /*A Affectation de la variable pmx_xxxx_texte3 */
   sprintf( vl_nom_var , "%stexte3" , vl_pref_var);
   sprintf(vl_msg, "|10|2|%s|texte3|%s", pa_nom, pa_ligne4);
 if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , pa_ligne4 , vl_nom_var );
		return;
	}
}

   /*A Affectation de la variable pmx_xxxx_texte4 */
   sprintf( vl_nom_var , "%stexte_dir" , vl_pref_var);
   sprintf(vl_msg, "|10|2|%s|texte_dir|%s", pa_nom, pa_ligne1);
   if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , pa_ligne1 , vl_nom_var );
		return;
	}
}


XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA01_raf_vag_PMVDATEX_Message" );

return;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un PMV picto
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA02_raf_vag_PMV_Picto(
          va_etat_picto,
          pa_result, 
          va_machine)
          
          XDY_Etat_Picto va_etat_picto;
          int *pa_result;
          char *va_machine;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_picto : Descriptif de l'etat du picto a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA02_raf_vag_PMV_Picto ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   float vl_val_num;
   char vl_panonceau[XDC_Lon_TxtPicto];
   char *vl_dollar;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA02_raf_vag_PMV_Picto" );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "pmv_%04d_" , va_etat_picto.Numero );
   
   /*A Affectation de la variable pmx_xxxx_texte_picto */
   sprintf( vl_nom_var , "%stexte_picto" , vl_pref_var);
   va_etat_picto.Texte[XDC_Lon_TextePicto-1]='\000';
   strcpy(vl_panonceau,va_etat_picto.Texte);
   if (strlen(vl_panonceau) > 0)
   {
       /* s'il y a un $ dans la chaine, c'est un pictogramme */
      vl_dollar=strstr(vl_panonceau,"$");
      if (vl_dollar!=NULL)
      {
         /* on retire les espaces */
         strcpy(vl_panonceau,vl_dollar);
         rtrim(vl_panonceau);
      }

     sprintf(vl_msg, "|10|2|%d|texte_picto|%s", va_etat_picto.Numero, vl_panonceau);
   }
else 
  sprintf(vl_msg, "|10|2|%d|texte_picto| ",va_etat_picto.Numero);
/*   if (canWeSend(vl_msg)) {*/
   /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
   canWeSend(vl_msg);
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , va_etat_picto.Texte, vl_nom_var );
		return;
	}
/*}*/
   
   /* Affectation de la variable pmx_xxxx_picto */
   sprintf( vl_nom_var , "%spicto" , vl_pref_var);
   vl_val_num = MSRA_No_Picto( va_etat_picto.Picto);
   sprintf(vl_msg, "|10|2|%d|picto|%d", va_etat_picto.Numero, (int)vl_val_num);
   if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num, vl_nom_var );
		return;
	}
}

/* PN : Inactivation PMV */
/* Si le PMV est inactiv���, alors il faut l'inactiv��� */
sprintf( vl_nom_var ,"pmv_%04d_inactif" , va_etat_picto.Numero);
if ( vl_val_num >= 1 )
{
   /*XZST_03EcritureTrace(   XZSTC_WARNING , "Inactiver %s = %f; "  , vl_nom_var , vl_val_num);
        sprintf(vl_msg, "|10|2|%d|activation|0", va_etat_picto.Numero);
      	if (send_message(vl_msg, CM_BUFF_SOCK) == -1)
           {
           XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'inactiver le pmv %d" , va_etat_picto.Numero );
           }*/
	/*MSOP06_Majinactiver_PMV(va_etat_picto.Numero,va_etat_picto.Texte);*/

}
/*A Mise a jour de la vue */

XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA02_raf_vag_PMV_Picto" );
*pa_result = XDC_OK;
return;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'une TRA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA03_raf_vag_TRA(
          va_etat_TRA,
          pa_result)
          
          XDY_EMes_RADT va_etat_TRA;
          int *pa_result;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_TRA : Descriptif de l'etat du TRA a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA03_raf_vag_TRA ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   char vl_val_texte[256];
   float vl_val_num,
   	 vl_valeur;
   float *pl_val_num;
   char *pl_val_texte;
   float vl_tendance;
   XZSMT_Horodate vl_horodate;
   XDY_Entier vl_div=0;
   int vl_Long_Elect=0;


   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA03_raf_vag_TRA" );
   
   /*A Si les donnees ne sont pas celles de la chaussee, on ne fait rien */
   if ( va_etat_TRA.Voie != XDC_RADT_CHAUSSEE )
   return;
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "radt_%04d%d_" , va_etat_TRA.Numero, va_etat_TRA.Sens );
   
   /*A Calcul de la tendance de la variable radt_xxxx_V*/
   sprintf( vl_nom_var , "%sV" , vl_pref_var);
   if ( MSRA_Calcul_tendance( vl_nom_var , va_etat_TRA.Mesures.ValV.Val , &vl_tendance ) == XDC_NOK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors du calcul de tendance de %s" , vl_nom_var );
      *pa_result = XDC_NOK;
      }
	
   /*A Affectation de la variable radt_xxxx_V */
   sprintf( vl_nom_var , "%sV" , vl_pref_var);
   if ( va_etat_TRA.Mesures.ValV.Indic == XDC_RADT_INCONNU )
      {
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
      }
   else
   if ( va_etat_TRA.Mesures.ValV.Val == -1 )
      {
      sprintf( vl_val_texte, "HS"  );
      vl_tendance = 0.0;
      }
   else
   if ( va_etat_TRA.Mesures.ValV.Indic == XDC_RADT_RECONST )
      {
      sprintf( vl_val_texte, "%d E" , va_etat_TRA.Mesures.ValV.Val );
      }
   else
      {
      sprintf( vl_val_texte, "%d" , va_etat_TRA.Mesures.ValV.Val );
      }
   sprintf( vl_nom_var , "%sV_tend" , vl_pref_var);
      
	sprintf(vl_msg, "|23|26|%d|%d|V|%s", va_etat_TRA.Numero,va_etat_TRA.Sens,vl_val_texte);
	if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK,"") == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag V" , vl_val_texte);
		*pa_result = XDC_NOK;
		return;
	}
	}
/*A Calcul de la tendance de la variable radt_xxxx_Q*/
sprintf( vl_nom_var , "%sQ" , vl_pref_var);
if ( MSRA_Calcul_tendance( vl_nom_var , va_etat_TRA.Mesures.ValQ.Val , &vl_tendance ) == XDC_NOK )
{
	XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors du calcul de tendance de %s" , vl_nom_var );
	*pa_result = XDC_NOK;
}

/*A Affectation de la variable radt_xxxx_Q */
sprintf( vl_nom_var , "%sQ" , vl_pref_var);
if ( va_etat_TRA.Mesures.ValQ.Indic == XDC_RADT_INCONNU )
{
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
}
   else
   if ( va_etat_TRA.Mesures.ValQ.Val == -1 )
      {
      sprintf( vl_val_texte, "HS"  );
      vl_tendance = 0.0;
      }
   else
   if ( va_etat_TRA.Mesures.ValQ.Indic == XDC_RADT_RECONST )
      {
      sprintf( vl_val_texte, "%d E" , va_etat_TRA.Mesures.ValQ.Val );
      }
   else
      {
      sprintf( vl_val_texte, "%d" , va_etat_TRA.Mesures.ValQ.Val );
      }
   sprintf( vl_nom_var , "%sQ_tend" , vl_pref_var);

	sprintf(vl_msg, "|23|26|%d|%d|Q|%s", va_etat_TRA.Numero,va_etat_TRA.Sens,vl_val_texte);
	if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag Q" , vl_val_texte);
		*pa_result = XDC_NOK;
		return;
	}
	}

/*A Calcul de la tendance TO */
sprintf( vl_nom_var , "%sTO" , vl_pref_var);
if ( MSRA_Calcul_tendance( vl_nom_var , va_etat_TRA.Mesures.ValTO.Val , &vl_tendance ) == XDC_NOK )
{
	XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors du calcul de tendance de %s" , vl_nom_var );
	*pa_result = XDC_NOK;
}
/*A Affectation de la variable radt_xxxx_TO */
sprintf( vl_nom_var , "%sTO" , vl_pref_var);
if ( va_etat_TRA.Mesures.ValTO.Indic == XDC_RADT_INCONNU )
{
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
}
   else
	if ( va_etat_TRA.Mesures.ValTO.Val == -1 )
	{
      	sprintf( vl_val_texte, "HS"  );
      	vl_tendance = 0.0;
	}
	else
   if ( va_etat_TRA.Mesures.ValTO.Indic == XDC_RADT_RECONST )
     {
      	sprintf( vl_val_texte, "%d E" , va_etat_TRA.Mesures.ValTO.Val );
     }
		else
		{
			sprintf( vl_val_texte, "%d" , va_etat_TRA.Mesures.ValTO.Val );
		}
sprintf( vl_nom_var , "%sTO_tend" , vl_pref_var);

sprintf(vl_msg, "|23|26|%d|%d|TO|%s", va_etat_TRA.Numero,va_etat_TRA.Sens,vl_val_texte);
	if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag TO" , vl_val_texte);
		*pa_result = XDC_NOK;
		return;
	}
	}

/*A Calcul de la tendance PL*/
sprintf( vl_nom_var , "%sPL" , vl_pref_var);
if ( MSRA_Calcul_tendance( vl_nom_var , va_etat_TRA.Mesures.ValPL.Val , &vl_tendance ) == XDC_NOK )
{
	XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors du calcul de tendance de %s" , vl_nom_var );
	*pa_result = XDC_NOK;
}

   /*A Affectation de la variable radt_xxxx_PL */
   sprintf( vl_nom_var , "%sPL" , vl_pref_var);
   if ( va_etat_TRA.Mesures.ValPL.Indic == XDC_RADT_INCONNU )
      {
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
      }
   else
   if ( va_etat_TRA.Mesures.ValPL.Val == -1 )
      {
      sprintf( vl_val_texte, "HS"  );
      vl_tendance = 0.0;
      }
   else
   if ( va_etat_TRA.Mesures.ValPL.Indic == XDC_RADT_RECONST )
      {
      sprintf( vl_val_texte, "%d E" , va_etat_TRA.Mesures.ValPL.Val );
      }
   else
      {
      sprintf( vl_val_texte, "%d" , va_etat_TRA.Mesures.ValPL.Val );
      }
   sprintf( vl_nom_var , "%sPL_tend" , vl_pref_var);

	sprintf(vl_msg, "|23|26|%d|%d|PL|%s", va_etat_TRA.Numero,va_etat_TRA.Sens,vl_val_texte);
	if (canWeSend(vl_msg)) {
   	if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
        {
          XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag PL" , vl_val_texte);
         *pa_result = XDC_NOK;
         return;
        }
}
   /*A Affectation de la variable radt_xxxx_horodate */
   sprintf( vl_nom_var , "%shorodate" , vl_pref_var);
   if ( XZSM_11ConversionHorodate( (double) va_etat_TRA.Horodate , &vl_horodate ) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , " :Impossible de convertir l'horodate %lf " , va_etat_TRA.Horodate );
      *pa_result = XDC_NOK;
      }
   /* suppression des secondes */
   vl_horodate[16]='\000';

	sprintf(vl_msg, "|23|26|%d|%d|H|%s", va_etat_TRA.Numero,va_etat_TRA.Sens,vl_horodate);
	if (canWeSend(vl_msg)) {
   	if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
        {
          XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag H" , vl_val_texte);
         *pa_result = XDC_NOK;
         return;
        }
	}

   /*A Calcul de la tendance de la variable radt_xxxx_DIV1*/
   sprintf( vl_nom_var , "%sDIV1" , vl_pref_var);
   
//   int vm_long_veh;
   vl_Long_Elect = vm_long_boucl + vm_long_veh;
   
   if ( ( 10 * va_etat_TRA.Mesures.ValTO.Val - vl_Long_Elect ) == 0 )
      vl_div=-1;
   else
      vl_div = ( 1000 * vm_long_boucl - 10 * va_etat_TRA.Mesures.ValTO.Val * vm_long_veh ) /
	  ( 10 * va_etat_TRA.Mesures.ValTO.Val - vl_Long_Elect );

   if ( MSRA_Calcul_tendance( vl_nom_var , vl_div , &vl_tendance ) == XDC_NOK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors du calcul de tendance de %s" , vl_nom_var );
      *pa_result = XDC_NOK;
      return;
      }
   /*A Affectation de la variable radt_xxxx_DIV1 */
   sprintf( vl_nom_var , "%sDIV1" , vl_pref_var);
   if ( ( va_etat_TRA.Mesures.ValTO.Indic == XDC_RADT_INCONNU ) || ( vl_div == -1 ) )
      {
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
      }
   else
   if ( va_etat_TRA.Mesures.ValTO.Val == -1 )
      {
      sprintf( vl_val_texte, "HS"  );
      vl_tendance = 0.0;
      }
   else
      {
      if ( va_etat_TRA.Mesures.ValTO.Val == 0 )
         vl_div = 0;
      if ( va_etat_TRA.Mesures.ValTO.Indic == XDC_RADT_RECONST  )
         {
         sprintf( vl_val_texte, "%d E" , vl_div );
         }
      else
         {
         sprintf( vl_val_texte, "%d" , vl_div );
         }
      }
      
   sprintf( vl_nom_var , "%sDIV1_tend" , vl_pref_var);
   sprintf(vl_msg, "|23|26|%d|%d|DIV|%s", va_etat_TRA.Numero,va_etat_TRA.Sens,vl_val_texte);
	if (canWeSend(vl_msg)) {
   	if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
        {
          XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag H" , vl_val_texte);
         *pa_result = XDC_NOK;
         return;
        }
}
   /*A Mise a jour de la vue */
   /*vl_valeur = *((float *) MSVU_Valeur_vag( "radt_visibilite" ) );*/
   XZST_03EcritureTrace(   XZSTC_DEBUG1 ,"IMSRA03_raf_vag_TRA: radt_visibilite=%f", vl_valeur);
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA03_raf_vag_TRA" );
   *pa_result = XDC_OK;
   return;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'une radt et/ou d'une courbe f(t)
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA03_raf_RADT(
          va_etat_TRA,
          pa_result)
          
          XDY_EMes_RADT va_etat_TRA;
          int *pa_result;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_TRA : Descriptif de l'etat du TRA a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA03_raf_RADT ";
   
   
   

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA03_raf_RADT" );
   
   *pa_result= XDC_OK;
   
   
   /*A Si les donnees  sont celles de la chaussee et que la station est animee ou que le panneau est ouvert alors mettre a jour les vags */
   //		   if ( ( va_etat_TRA.Voie == XDC_RADT_CHAUSSEE ) &&
   //        ( (MSRA_RADT_Animee( va_etat_TRA.Numero, va_etat_TRA.Sens ) == XDC_OK ) || (MSOP_Panneau_Abonne( va_etat_TRA.Numero, va_etat_TRA.Sens ) == XDC_OK ) )
   if ( va_etat_TRA.Voie == XDC_RADT_CHAUSSEE )
        {
        MSRA03_raf_vag_TRA(va_etat_TRA, pa_result);
        }
   /*A S'il existe une courbe avec les donnees identiques alors la rafraichir */
   if ( MSCO_Existe_Courbe_Abt(
               va_etat_TRA.Numero,
               va_etat_TRA.Voie,
               va_etat_TRA.Sens ) == XDC_OUI )
        {
        MSCO02_raf_vag( va_etat_TRA, pa_result );
        }


   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA03_raf_RADT : %d", *pa_result );
   return;
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement de la  vag de Distance Inter_Vehicules d'une radt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA03_raf_vag_DIV(
          va_etat_DIV,
          pa_result)
          
          XDY_EMes_Classe_DIV va_etat_DIV;
          int *pa_result;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_DIV : Descriptif de l'etat de la DIV
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA03_raf_vag_DIV ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   char vl_val_texte[256];
   float vl_val_num;
   float vl_tendance;
   XZSMT_Horodate vl_horodate;
   

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA03_raf_vag_DIV" );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "radt_%04d%d_" , va_etat_DIV.Numero, va_etat_DIV.Sens );
   
   /*A Calcul de la tendance de la variable radt_xxxx_DIV1*/
   sprintf( vl_nom_var , "%sDIV1" , vl_pref_var);
   if ( MSRA_Calcul_tendance( vl_nom_var , va_etat_DIV.DIVcl1 , &vl_tendance ) == XDC_NOK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors du calcul de tendance de %s" , vl_nom_var );
      *pa_result = XDC_NOK;
      return;
      }
   /*A Affectation de la variable radt_xxxx_V */
   sprintf( vl_nom_var , "%sDIV1" , vl_pref_var);
   if ( va_etat_DIV.DIVcl1 == XDC_RADT_DIV_INCONNU )
      {
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
      }
   else
      {
      sprintf( vl_val_texte, "%d" , va_etat_DIV.DIVcl1 );
      }
      
   sprintf( vl_nom_var , "%sDIV1_tend" , vl_pref_var);

   /*A Calcul de la tendance de la variable radt_xxxx_DIV2*/
   sprintf( vl_nom_var , "%sDIV2" , vl_pref_var);
   if ( MSRA_Calcul_tendance( vl_nom_var , va_etat_DIV.DIVcl2 , &vl_tendance ) == XDC_NOK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors du calcul de tendance de %s" , vl_nom_var );
      *pa_result = XDC_NOK;
      return;
      }
   /*A Affectation de la variable radt_xxxx_DIV2 */
   sprintf( vl_nom_var , "%sDIV2" , vl_pref_var);
   if ( va_etat_DIV.DIVcl2 == XDC_RADT_DIV_INCONNU )
      {
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
      }
   else
      {
      sprintf( vl_val_texte, "%d" , va_etat_DIV.DIVcl2 );
      }
   sprintf( vl_nom_var , "%sDIV2_tend" , vl_pref_var);

   /*A Calcul de la tendance de la variable radt_xxxx_DIV3*/
   sprintf( vl_nom_var , "%sDIV3" , vl_pref_var);
   if ( MSRA_Calcul_tendance( vl_nom_var , va_etat_DIV.DIVcl3 , &vl_tendance ) == XDC_NOK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors du calcul de tendance de %s" , vl_nom_var );
      *pa_result = XDC_NOK;
      return;
      }
   /*A Affectation de la variable radt_xxxx_DIV3 */
   sprintf( vl_nom_var , "%sDIV3" , vl_pref_var);
   if ( va_etat_DIV.DIVcl3 == XDC_RADT_DIV_INCONNU )
      {
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
      }
   else
      {
      sprintf( vl_val_texte, "%d" , va_etat_DIV.DIVcl3 );
      }
   sprintf( vl_nom_var , "%sDIV3_tend" , vl_pref_var);

   /*A Calcul de la tendance de la variable radt_xxxx_DIV4*/
   sprintf( vl_nom_var , "%sDIV4" , vl_pref_var);
   if ( MSRA_Calcul_tendance( vl_nom_var , va_etat_DIV.DIVcl4 , &vl_tendance ) == XDC_NOK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors du calcul de tendance de %s" , vl_nom_var );
      *pa_result = XDC_NOK;
      return;
      }
   /*A Affectation de la variable radt_xxxx_DIV4 */
   sprintf( vl_nom_var , "%sDIV4" , vl_pref_var);
   if ( va_etat_DIV.DIVcl4 == XDC_RADT_DIV_INCONNU )
      {
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
      }
   else
      {
      sprintf( vl_val_texte, "%d" , va_etat_DIV.DIVcl4 );
      }
   sprintf( vl_nom_var , "%sDIV4_tend" , vl_pref_var);

   /*A Calcul de la tendance de la variable radt_xxxx_DIV5*/
   sprintf( vl_nom_var , "%sDIV5" , vl_pref_var);
   if ( MSRA_Calcul_tendance( vl_nom_var , va_etat_DIV.DIVcl5 , &vl_tendance ) == XDC_NOK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors du calcul de tendance de %s" , vl_nom_var );
      *pa_result = XDC_NOK;
      return;
      }
   /*A Affectation de la variable radt_xxxx_DIV5 */
   sprintf( vl_nom_var , "%sDIV5" , vl_pref_var);
   if ( va_etat_DIV.DIVcl5 == XDC_RADT_DIV_INCONNU )
      {
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
      }
   else
      {
      sprintf( vl_val_texte, "%d" , va_etat_DIV.DIVcl5 );
      }
   sprintf( vl_nom_var , "%sDIV5_tend" , vl_pref_var);

   /*A Affectation de la variable radt_xxxx_horodate_DIV */
   sprintf( vl_nom_var , "%shorodate_DIV" , vl_pref_var);
   if ( XZSM_11ConversionHorodate( (double) va_etat_DIV.Horodate , &vl_horodate ) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , " :Impossible de convertir l'horodate %lf " , va_etat_DIV.Horodate );
      *pa_result = XDC_NOK;
      return;
      }
   /* suppression des secondes */
   vl_horodate[16]='\000';

   /*A Mise a jour de la vue */

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA03_raf_vag_DIV" );
   *pa_result = XDC_OK;
   return;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Calcule de la tendance de variation d'une variable 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSRA_Calcul_tendance(
          va_nom_var,
          va_valeur,
          pa_tendance)
          
          char *  va_nom_var;
          XDY_Mot va_valeur;
          float  * pa_tendance;

/*
* ARGUMENTS EN ENTREE :
*   va_nom_var : Nom de la variable pour laquelle on veut calculer la tendance
*   va_valeur : Valeur de la nouvelle valeur de la variable
*   
*
*
* ARGUMENTS EN SORTIE :
*   pa_tendance : Pointeur sur la tendance a calculer
*   
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA_Calcul_tendance ";
   XDY_Mot vl_val_num;
   char *pl_val_texte;
   
return (XDC_OK);

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Calcul_tendance va_nom_var:%s; va_leur:%d; " ,va_nom_var, va_valeur);
   
   
   vl_val_num = atoi( pl_val_texte );
   if ( vl_val_num == va_valeur )
      *pa_tendance = 0;
   else
      if ( vl_val_num > va_valeur )
         *pa_tendance = -1;
      else
         *pa_tendance =  1;
         
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Calcul_tendance : %f" , *pa_tendance);
   return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'une TRA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA03_raf_vag_NivTRA(
          va_etat_Niv,
          pa_result, 
          va_machine)
          
          XDY_ENiveau_Service va_etat_Niv;
          int *pa_result;
          char *va_machine;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_Niv : Descriptif de l'etat de la zone RADT a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA03_raf_vag_NivTRA ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   char vl_val_texte[256];
   float vl_val_num;

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA03_raf_vag_NivTRA" );
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : NUM %d, sens %d, niv %d",
   			va_etat_Niv.Numero,
			va_etat_Niv.Sens,
			va_etat_Niv.NivService);
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "radt_%04d%d_" , va_etat_Niv.Numero, va_etat_Niv.Sens );
   
   /*A Affectation de la variable radt_xxxx_tra */
   sprintf( vl_nom_var , "%stra" , vl_pref_var);
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : vl_nom_var %s",vl_nom_var);
   
   switch ( va_etat_Niv.NivService )
      {
      case XZERC_NIV_FLUIDE :
          vl_val_num = 0.37;
          break;
      case XZERC_NIV_PRESATURE :
          vl_val_num = 0.62;
          break;
      case XZERC_NIV_SATURE :
          vl_val_num = 0.87;
          break;
      default:
          XZST_03EcritureTrace(   XZSTC_DEBUG1 , "Niveau de trafic inconnu : %d",va_etat_Niv.NivService);
          vl_val_num = 0.0;
          break;
      }
   /*A Mise a jour de la vue */
   /*sprintf(vl_msg, "|11|%d|%d|%g|%s",  va_etat_Niv.Numero, va_etat_Niv.Sens, vl_val_num, vl_nom_var);*/
   sprintf(vl_msg, "|11|%d|%d|%g",  va_etat_Niv.Numero, va_etat_Niv.Sens, vl_val_num );
   if (canWeSend(vl_msg)) {
   if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la zone %d pour le sens %d" , vl_val_num ,  va_etat_Niv.Numero, va_etat_Niv.Sens);
      *pa_result = XDC_NOK;
      return;
      }
     }

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA03_raf_vag_NivTRA" );
   *pa_result = XDC_OK;
   return;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un NAV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA04_raf_vag_NAV(
          va_etat_NAV,
          pa_result, 
          va_machine)
          
          XZEAT_Etat_NAV  va_etat_NAV;
          int *pa_result;
          char *va_machine;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_NAV : Descriptif de l'etat du NAV a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA04_raf_vag_NAV ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   int vl_val_num_b;
   int vl_val_num_p;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA04_raf_vag_NAV" );
   
   
   if ( va_etat_NAV.TypeEqt == XDC_EQT_NAV )
   {
   /*A  Construction du prefise des variables associees a la nav */
   sprintf( vl_pref_var , "nav_%04d_" , va_etat_NAV.Etat.Numero );
   
   if ( ( va_etat_NAV.Etat.Modules[0][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[1][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[2][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[3][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[4][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[5][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[6][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[7][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[8][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[9][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[10][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[11][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[12][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[13][0] != XZENC_INACTIF ) )
      vl_val_num_p = 1;
   else
      vl_val_num_p = 0;
      
   if ( ( va_etat_NAV.Etat.Modules[14][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[15][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[16][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[17][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[18][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[19][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[20][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[21][0] != XZENC_INACTIF ) )
      vl_val_num_b = 1;
   else
      vl_val_num_b = 0;
      
     
   /*A Affectation de la variable de biseaux */
   sprintf( vl_nom_var , "%sbiseaux" , vl_pref_var);
   sprintf(vl_msg, "|10|1|%d|biseau|%d", va_etat_NAV.Etat.Numero, vl_val_num_b);
   /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
   canWeSend(vl_msg);
   if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num_b, vl_nom_var );
      *pa_result = XDC_NOK;
      return;
      }

   /*A Affectation de la variable de presignalisation */
   sprintf( vl_nom_var , "%spresignal" , vl_pref_var);
   sprintf(vl_msg, "|10|1|%d|presignal|%d", va_etat_NAV.Etat.Numero, vl_val_num_p);
   /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
   canWeSend(vl_msg);
   if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num_p, vl_nom_var );
      *pa_result = XDC_NOK;
      return;
      }
   }
   else
   {
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "ferm_%04d_" , va_etat_NAV.Etat.Numero );
   
   /*A Affectation de la variable  */
   sprintf( vl_nom_var , "%sfonct" , vl_pref_var);
   if ( ( va_etat_NAV.Etat.Modules[0][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[1][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[2][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[3][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[4][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[5][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[6][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[7][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[8][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[9][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[10][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[11][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[12][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[13][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[14][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[15][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[16][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[17][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[18][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[19][0] != XZENC_INACTIF ) ||
        ( va_etat_NAV.Etat.Modules[20][0] != XZENC_INACTIF ) || ( va_etat_NAV.Etat.Modules[21][0] != XZENC_INACTIF ) )
      vl_val_num_p = 1;
   else
      vl_val_num_p = 0;
      
   }
   /*A Mise a jour de la vue */

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA04_raf_vag_NAV" );
   *pa_result = XDC_OK;
   return;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un DAI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA05_raf_vag_DAI(
          va_etat_DAI,
          pa_result)
          
          XDY_EMes_DAI va_etat_DAI;
          int *pa_result;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_ DAI: Descriptif de l'etat du DAI a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA05_raf_vag_DAI ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   char vl_texte[256];
   XZSMT_Horodate vl_horodate;
   float vl_val_num;
   int i;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA05_raf_vag_DAI" );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "dai_%04d_" , va_etat_DAI.Numero );
   
   /*A Affectation de la variable  */
   sprintf( vl_nom_var , "%shorodate" , vl_pref_var);
   if ( XZSM_11ConversionHorodate( (double) va_etat_DAI.Horodate , &vl_horodate ) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , " :Impossible de convertir l'horodate %lf " , va_etat_DAI.Horodate );
      *pa_result = XDC_NOK;
      return;
      }
   sprintf( vl_nom_var , "%sHorodate" , vl_pref_var);
      
   sprintf( vl_nom_var , "%sTO" , vl_pref_var);

   sprintf( vl_nom_var , "%sTO" , vl_pref_var);
   sprintf( vl_texte , "%d" , va_etat_DAI.TO );

   sprintf( vl_nom_var , "%sV" , vl_pref_var);
   sprintf( vl_texte , "%d" , va_etat_DAI.VitesseMoyenne);
   for ( i = 0; i < 5 ; i++)
      {
      /*Si les donnees de Voie sont negatives , elles sont invalides */
      sprintf( vl_nom_var , "%sV%d" , vl_pref_var, i +1 );
      if ( va_etat_DAI.VitesseVoie[i] < 0 )
         strcpy( vl_texte , "" );
      else
         sprintf( vl_texte , "%d" , va_etat_DAI.VitesseVoie[i] );
         
      sprintf( vl_nom_var , "%sL%d" , vl_pref_var, i +1 );
      if ( va_etat_DAI.LongueurVoie[i]  < 0)
         strcpy( vl_texte , "" );
      else
         sprintf( vl_texte , "%d" , va_etat_DAI.LongueurVoie[i] );
      }

   /*A Mise a jour de la vue */
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA05_raf_vag_DAI" );
   *pa_result = XDC_OK;
   return;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement dela  vag alerte d'un DAI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSRA05_raf_vag_DAI_alerte(
          va_Num_DAI,
          va_Type_Alerte, 
          va_machine)
          
          XDY_Eqt va_Num_DAI;
          XDY_TypeAlerte va_Type_Alerte;
          char *va_machine;
          

/*
* ARGUMENTS EN ENTREE :
*   va_etat_ DAI: Descriptif de l'etat du DAI a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA05_raf_vag_DAI_Alerte ";
   char vl_nom_var[256];
   char vl_nom_objet[256];
   char vl_nom_objet_txt[256];
   char vl_pref_var[256];
   float vl_val_num;
   float vl_visi_eqt_activee=1;
//   OBJECT vl_tt;
   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA05_raf_vag_DAI_Alerte" );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "dai_%04d_" , va_Num_DAI );
   sprintf( vl_nom_objet , "dai_%04d" , va_Num_DAI );
   sprintf( vl_nom_objet_txt , "tdai_%04d" , va_Num_DAI );
   
   /*A Affectation de la variable  */
   sprintf( vl_nom_var , "%salerte" , vl_pref_var);
   vl_val_num = (float)va_Type_Alerte;
   
   sprintf(vl_msg, "|10|4|%d|alerte|%d", va_Num_DAI, vl_val_num);
   /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
   canWeSend(vl_msg);
   if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num, vl_nom_var );
      
      return;
      }
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA05_raf_vag_DAI_Alerte" );
   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un PAU
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA06_raf_vag_PAU(
          va_etat_PAU,
          pa_result, 
          va_machine)
          
          XDY_Etat_PAU va_etat_PAU;
          int *pa_result;
          char *va_machine;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_PAU : Descriptif de l'etat du  a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA06_raf_vag_PAU ";
   char vl_nom_var[256];
   char vl_nom_objet[256];
   char vl_pref_var[256];
   float vl_val_num;
   

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA06_raf_vag_PAU" );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "pau_%04d_" , va_etat_PAU.Numero );
   sprintf( vl_nom_objet , "pau_%04d" , va_etat_PAU.Numero );
   
   /*A Affectation de la variable  */
   sprintf( vl_nom_var , "%sfonct" , vl_pref_var);
   switch ( va_etat_PAU.EtatComRAU )
      {
      case XZEUC_ETAT_REPOS :
          vl_val_num = 0.12;
             /*vl_tt = vm_tt_pau;
             if ( vl_tt != NULL )
                MSVU_Ajout_Retire_Visi( vl_nom_var , vl_nom_objet , "pau_visibilite" , &vl_tt );*/
          break;
      case XZEUC_ETAT_APPEL :
          vl_val_num = 0.37;
         /*A Inhibition du dyn de visi sur l'objet */
          /*vl_tt = NULL;
          if ( MSVU_Ajout_Retire_Visi( vl_nom_var , vl_nom_objet , "pau_visibilite" , &vl_tt ) != XDC_OK )
             {
             XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de recuperer la tt de %s " , vl_nom_var );
             return;
             }
          vm_tt_pau = 1;*/
          break;
      case XZEUC_ETAT_COMM :
          vl_val_num = 0.62;
          break;
      case XZEUC_ETAT_GARDE :
          vl_val_num = 0.87;
          break;
      default :
          XZST_03EcritureTrace(   XZSTC_WARNING, "OUT : MSRA06_raf_vag_PAU etat inconnu " );
          *pa_result = XDC_OK;
          return;
          break;
      }

   /*A Mise a jour de la vue */
   sprintf(vl_msg, "|10|14|%d|etatAppel|%f", va_etat_PAU.Numero, vl_val_num);
   /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
   canWeSend(vl_msg);
   if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num, vl_nom_var );
      *pa_result = XDC_NOK;
      return;
      }
   

   

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA06_raf_vag_PAU" );
   *pa_result = XDC_OK;
   return;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un CAMERA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA07_raf_vag_CAMERA(
          va_etat_CAMERA,
          pa_result)
          
          XDY_Etat_Camera va_etat_CAMERA;
          int *pa_result;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_CAMERA : Descriptif de l'etat du CAMERA a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA07_raf_vag_CAMERA ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   float vl_val_num;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA07_raf_vag_CAMERA" );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "_%04d_" , va_etat_CAMERA.Numero );
   
   /*A Affectation de la variable  */
   sprintf( vl_nom_var , "%s" , vl_pref_var);

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA07_raf_vag_CAMERA" );
   *pa_result = XDC_OK;
   return;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un LIT
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSRA08_raf_vag_LIT(
          va_etat_LIT)
          
          XDY_Capt_Tube va_etat_LIT;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_LIT : Descriptif de l'etat du LIT a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA08_raf_vag_LIT ";
   char vl_nom_var[256];
   char vl_nom_objet[256];
   char vl_pref_var[256];
   float vl_val_num;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA08_raf_vag_LIT" );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "tub_%04d_" , va_etat_LIT.NoTub_Ech );
   sprintf( vl_nom_objet , "lit_%04d" , va_etat_LIT.NoTub_Ech );
   
   /*A Affectation de la variable  */
   sprintf( vl_nom_var , "%salerte" , vl_pref_var);
   
   vl_val_num = (float) va_etat_LIT.Etat;
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA08_raf_vag_LIT" );
   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'une station STRADA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA09_raf_vag_STRADA(
          va_etat_TRA )
          
          XDY_EMes_RADT va_etat_TRA;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_TRA : Descriptif de l'etat du TRA a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA09_raf_vag_STRADA ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   char vl_val_texte[256];
   float vl_val_num,
   	 vl_valeur;
   float *pl_val_num;
   char *pl_val_texte;
   float vl_tendance;
   XZSMT_Horodate vl_horodate;
   XDY_Entier vl_div=0;
   int vl_Long_Elect=0;
   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA09_raf_vag_STRADA" );
   
   /*A Si les donnees ne sont pas celles de la chaussee, on ne fait rien */
   if ( va_etat_TRA.Voie != XDC_RADT_CHAUSSEE )
      return;
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "strada_%04d%d_" , va_etat_TRA.Numero, va_etat_TRA.Sens );
   
   /*A Affectation de la variable strada_xxxx_V */
   sprintf( vl_nom_var , "%sV" , vl_pref_var);
   if ( va_etat_TRA.Mesures.ValV.Indic == XDC_RADT_INCONNU )
      {
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
      }
   else
      {
      sprintf( vl_val_texte, "%d" , va_etat_TRA.Mesures.ValV.Val );
      }
      
      

   /*A Calcul de la tendance de la variable radt_xxxx_Q*/

   /*A Affectation de la variable radt_xxxx_Q */
   sprintf( vl_nom_var , "%sQ" , vl_pref_var);
   if ( va_etat_TRA.Mesures.ValQ.Indic == XDC_RADT_INCONNU )
      {
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
      }
   else
      {
      sprintf( vl_val_texte, "%d" , va_etat_TRA.Mesures.ValQ.Val );
      }
      


   /*A Calcul de la tendance TO */
   /*A Affectation de la variable radt_xxxx_TO */
   sprintf( vl_nom_var , "%sTO" , vl_pref_var);
   if ( va_etat_TRA.Mesures.ValTO.Indic == XDC_RADT_INCONNU )
      {
      sprintf( vl_val_texte, "?"  );
      vl_tendance = 0.0;
      }
   else
      {
      sprintf( vl_val_texte, "%d" , va_etat_TRA.Mesures.ValTO.Val );
      }
      
      
   /*A Affectation de la variable radt_xxxx_horodate */
   sprintf( vl_nom_var , "%shorodate" , vl_pref_var);
   if ( XZSM_11ConversionHorodate( (double) va_etat_TRA.Horodate , &vl_horodate ) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , " :Impossible de convertir l'horodate %lf " , va_etat_TRA.Horodate );
      }
   /* suppression des secondes */
   vl_horodate[16]='\000';

   /*A Mise a jour de la vue */

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA09_raf_vag_STRADA" );
   return;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'une station strada et/ou d'une courbe f(t)
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA09_raf_STRADA(
          va_etat_TRA
          )
          
          XDY_EMes_RADT va_etat_TRA;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_TRA : Descriptif de l'etat du TRA a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA09_raf_STRADA ";
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA09_raf_STRADA" );
   
   /*A Si les donnees  ont celles de la chaussee alors mettre a jour les vags et la courbe si elle existe*/
   if ( va_etat_TRA.Voie == XDC_RADT_CHAUSSEE ) 
        {
        MSRA09_raf_vag_STRADA(va_etat_TRA );
        }




   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA09_raf_STRADA");
   return;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un FAV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA10_raf_vag_FAV(
          va_etat_FAV,
          pa_result)
          
          XDY_Etat_FAV va_etat_FAV;
          int *pa_result;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_FAV : Descriptif de l'etat du FAV a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA10_raf_vag_FAV ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   float vl_val_num;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA10_raf_vag_FAV" );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "_%04d_" , va_etat_FAV.Numero );
   
   /*A Affectation de la variable  */
   sprintf( vl_nom_var , "%s" , vl_pref_var);

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA10_raf_vag_FAV" );
   *pa_result = XDC_OK;
   return;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un Panneaux
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA11_raf_vag_Panneaux(
          va_etat_Panneaux,
          pa_result, 
          va_machine)
          
          XZEAT_Panneau va_etat_Panneaux;
          int *pa_result;
          char *va_machine;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_Panneaux : Descriptif de l'etat du Panneaux a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA11_raf_vag_Panneaux ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   float vl_val_num;

   XZST_03EcritureTrace(   XZSTC_INFO, "IN  : MSRA11_raf_vag_Panneaux" );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "fav_%04d_" , va_etat_Panneaux.NumPanneau);
   
   /*A Affectation de la variable  */
   sprintf( vl_nom_var , "%sfonct" , vl_pref_var);
   
   /*A Si Discordance panneau != 0 alors on eteint le panneau */
   if ( va_etat_Panneaux.EtatDisc != 0 )
   	vl_val_num = 0.0;
   else
   /*A Sinon on l'anime ... */
        vl_val_num = 0.166 + ((float) va_etat_Panneaux.EtatPanneau)/3.0;
        
   /*A Mise a jour de la vue */
   sprintf(vl_msg, "|10|20|%d|etat|%g", va_etat_Panneaux.NumPanneau, vl_val_num);
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA11_raf_vag_Panneaux %s %f",vl_nom_var,vl_val_num );
   if (canWeSend(vl_msg)) {
   if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num, vl_nom_var );
      *pa_result = XDC_NOK;
      return;
      }
   }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA11_raf_vag_Panneaux" );
   *pa_result = XDC_OK;
   return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un Panneaux
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA17_raf_vag_EqtSigGTCRenovee(
          XZEAT_Equipement va_etat,int *pa_resultat, char *va_machine)
{
	static char *version = "@(#)isbi_sra_rafobj.c	 :MSRA17_raf_vag_EqtSigGTCRenovee ";
	char vl_nom_var[256];
	char vl_pref_var[256];
	float vl_val_num;

	XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA17_raf_vag_EqtSigGTCRenovee" );
	XZST_03EcritureTrace(XZSTC_DEBUG1,"IN -- : %d %d %d %d %d %d %d %d",
							va_etat.Domaine,
							va_etat.TypeEqt,
							va_etat.NumEqt,
							va_etat.NumEquipement,
							va_etat.EtatEquipement,
							va_etat.EtatTechnique,
							va_etat.Mode,
							va_etat.EtatDisc);   
	if (va_etat.TypeEqt == XDC_EQT_TUB)
	{
			if (va_etat.Domaine == XDC_EQT_SIG)
					{
if (((va_etat.NumEqt==37)||(va_etat.NumEqt==38))&& (va_etat.EtatEquipement==1))
	va_etat.EtatEquipement=3;
	XZST_03EcritureTrace(XZSTC_WARNING,"IN -MSRA17_raf_vag_EqtSigGTCRenovee- : %d %d %d %d %d %d %d %d",
							va_etat.Domaine,
							va_etat.TypeEqt,
							va_etat.NumEqt,
							va_etat.NumEquipement,
							va_etat.EtatEquipement,
							va_etat.EtatTechnique,
							va_etat.Mode,
							va_etat.EtatDisc);   
		/*A  Construction du prefise des variables associees au pmv */
		sprintf( vl_pref_var , "fav_%04d_" , va_etat.NumEquipement);

		/*A Affectation de la variable  */
		sprintf( vl_nom_var , "%sfonct" , vl_pref_var);

		/*A Si Discordance panneau != 0 alors on eteint le panneau */
		if ( va_etat.EtatDisc != 0 )
		{
			vl_val_num = 0.0;
		}
		else
		{
			/*A Sinon on l'anime ... */
			vl_val_num = 0.166 + ((float) (va_etat.EtatEquipement-1))/3.0;
		}
   sprintf(vl_msg, "|10|20|%d|etat|%g", va_etat.NumEquipement, vl_val_num);
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA17_raf_vag_EqtSigGTCRenovee %s %f",vl_nom_var,vl_val_num );
   if (canWeSend(vl_msg)) {
   if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num, vl_nom_var );
      return;
      }
	}
	}
	}
	else
	{
		XZST_03EcritureTrace(   XZSTC_FONCTION, "MSRA17_raf_vag_EqtSigGTCRenovee : le type d'ouvrage n'est pas un tube : %d", va_etat.TypeEqt );
	}
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA17_raf_vag_EqtSigGTCRenovee" );
	*pa_resultat = XDC_OK;
	return;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un Extincteur
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA12_raf_vag_Extincteur(
          va_etat_Extincteur,
          pa_result)
          
          XDY_Capt_Tube va_etat_Extincteur;
          int *pa_result;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_Extincteur : Descriptif de l'etat de l'extincteur a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA12_raf_vag_Extincteur ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   float vl_val_num;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN MSRA12_raf_vag_Extincteur : " );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "tub_%04d_" , va_etat_Extincteur.NoTub_Ech );
   
   /*A Affectation de la variable  */
   sprintf( vl_nom_var , "%salerte" , vl_pref_var);
   vl_val_num = (float) va_etat_Extincteur.Etat;


   /*A Mise a jour de la vue */

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT MSRA12_raf_vag_Extincteur: " );
   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un Panneau d'alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA13_raf_vag_Pan_Alerte(
          va_etat_picto,
          pa_result, 
          va_machine)
          
          XDY_Etat_PAL va_etat_picto;
          int *pa_result;
          char *va_machine;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_picto : Descriptif de l'etat du picto a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA13_raf_vag_Pan_Alerte ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   int vl_val_num;
   int i;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA13_raf_vag_Pan_Alerte" );
   
   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "pan_%04d_" , va_etat_picto.Numero );
   
   *pa_result = XDC_OK;
   /* Affectation de la variable pan_xxxx_picto */
   vl_val_num = 0;
   sprintf( vl_nom_var , "%smod%d" , vl_pref_var,0);
   for (i=0;i < XDC_Lon_Nb_Mod_Pal;i++)
   {
      /*sprintf( vl_nom_var , "%smod%d" , vl_pref_var,i);*/
      if (va_etat_picto.Modules[i][0]!=XZENC_INACTIF) 
      {
      /*if (strcmp((char *)(va_etat_picto.Modules[i]),(char *)XZENC_INACTIF))*/
      vl_val_num = 1;
      break;
      }
  }
  /* vl_val_num = va_etat_picto.Modules[i];*/
   
   /*A Mise a jour de la vue */
   XZST_03EcritureTrace(XZSTC_DEBUG1, "vl_etat_picto.Modules[0]=[%s]",va_etat_picto.Modules[0]);
   XZST_03EcritureTrace(XZSTC_DEBUG1, "vl_etat_picto.Modules[1]=[%s]",va_etat_picto.Modules[1]);
   XZST_03EcritureTrace(XZSTC_DEBUG1, "vl_etat_picto.Modules[2]=[%s]",va_etat_picto.Modules[2]);
   XZST_03EcritureTrace(XZSTC_DEBUG1, "vl_etat_picto.Modules[3]=[%s]",va_etat_picto.Modules[3]);
   XZST_03EcritureTrace(XZSTC_DEBUG1, "vl_etat_picto.Modules[4]=[%s]",va_etat_picto.Modules[4]);
   XZST_03EcritureTrace(XZSTC_DEBUG1, "vl_etat_picto.Modules[5]=[%s]",va_etat_picto.Modules[5]);
   XZST_03EcritureTrace(XZSTC_DEBUG1, "vl_etat_picto.Modules[6]=[%s]",va_etat_picto.Modules[6]);
   XZST_03EcritureTrace(XZSTC_DEBUG1, "vl_etat_picto.Modules[7]=[%s]",va_etat_picto.Modules[7]);
   for (i=0;i < XDC_Lon_Nb_Mod_Pal;i++)
   {
      sprintf(vl_msg, "|10|45|%d|module%d|%s", va_etat_picto.Numero, i, va_etat_picto.Modules[i]);

      if (canWeSend(vl_msg)) 
      {
         if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num, vl_nom_var );
            *pa_result = XDC_NOK;
            return;
         }
      }

   }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA13_raf_vag_Pan_Alerte" );
   return;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement de la vag journuit
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_raf_journuit(
          va_etat_journuit,
          pa_result)
          
          XZEAT_JourNuit va_etat_journuit;
          int *pa_result;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_journuit : Descriptif de l'etat journuit
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA_raf_journuit ";
   char vl_nom_var[256];
   char vl_pref_var[256];
   float vl_val_num;

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_raf_journuit" );
   
   /*A Affectation de la variable  */
   strcpy( vl_nom_var , "journuit");
   switch ( va_etat_journuit.JourNuit )
      {
      case XDC_JOUR :
         vl_val_num =1;
         break;
      case XDC_NUIT :
         vl_val_num =0;
         break;
      default :
         XZST_03EcritureTrace(   XZSTC_WARNING , "valeur inattendu : %d" , va_etat_journuit.JourNuit );
         return ;
      }


   /*A Mise a jour de la vue */
   
   

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA11_raf_journuit" );
   return;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement de la vag affichant la sequence de signalisation du tunnel
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_raf_SeqTunnel(
          va_etat_seq,
          pa_result)
          
          XZEAT_Sequence va_etat_seq;
          int *pa_result;

/*
* ARGUMENTS EN ENTREE :
*   va_seq : Descriptif de l'etat journuit
*
*
* ARGUMENTS EN SORTIE :
*   pa_result : code de retour  XDC_OK ou XDC_NOK
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA_raf_SeqTunnel ";
   char vl_nom_var[256];
   char vl_val_text[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_raf_SeqTunnel" );
   
   /*A Affectation de la variable  */
   sprintf( vl_nom_var , "signal_%04d_scenario" ,va_etat_seq.NumEqt);
   
   /*A Recherche du lib du no de seq recupere */
   if ( MSRA_NomSequence( CM_SEQ_TUN,  va_etat_seq.SeqCourante , vl_val_text ) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_INFO , "Impossible de trouver la sequence no %d", va_etat_seq.SeqCourante );
      *pa_result=XDC_NOK;
      return ;
      }

   /*A Mise a jour de la vue */
   
   

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_raf_SeqTunnel" );
   *pa_result=XDC_OK;
   return;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement de la vag affichant la sequence de signalisation renovee
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_raf_SeqGTCRenovee(
          va_etat_seq,
          pa_result)
          
          XZEAT_Sequence va_etat_seq;
          int *pa_result;

/*
* ARGUMENTS EN ENTREE :
*   va_seq : Descriptif de l'etat journuit
*
*
* ARGUMENTS EN SORTIE :
*   pa_result : code de retour  XDC_OK ou XDC_NOK
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA_raf_SeqTunnel ";
	char vl_nom_var[256];
	char vl_val_text[256];

	XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_raf_SeqGTCRenovee : TypeEqt : %d - NumOuvrage : %d - NumSequence : %d ",
							va_etat_seq.TypeEqt,
							va_etat_seq.NumEqt,
							va_etat_seq.SeqCourante);

	/*A Affectation de la variable  */
	if (va_etat_seq.TypeEqt == XDC_EQT_TUB)
	{
		sprintf( vl_nom_var , "signal_%04d_scenario" ,va_etat_seq.NumEqt);
	} 
	else 
	{
		sprintf( vl_nom_var , "signale_%04d_scenario" ,va_etat_seq.NumEqt);
	}

	/*A Recherche du lib du no de seq recupere */
	if (va_etat_seq.TypeEqt == XDC_EQT_TUB)
	{
		if ( MSRA_NomSequence( CM_SEQ_TUN,  va_etat_seq.SeqCourante , vl_val_text ) != XDC_OK )
		{
			XZST_03EcritureTrace(   XZSTC_INFO , "Impossible de trouver la sequence no %d", va_etat_seq.SeqCourante );
			*pa_result=XDC_NOK;
			return ;
		}
	}
	else 
	{
		if ( MSRA_NomSequence( CM_SEQ_ECH,  va_etat_seq.SeqCourante , vl_val_text ) != XDC_OK )
		{
			XZST_03EcritureTrace(   XZSTC_INFO , "Impossible de trouver la sequence no %d", va_etat_seq.SeqCourante );
			*pa_result=XDC_NOK;
			return ;
		}
	}

	/*A Mise a jour de la vue */



	XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA_raf_SeqTunnel" );
	*pa_result=XDC_OK;
	return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement de la vag affichant la sequence de signalisation d'echangeur
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_raf_SeqEch(
          va_etat_seq,
          pa_result)
          
          XZEAT_Sequence va_etat_seq;
          int *pa_result;

/*
* ARGUMENTS EN ENTREE :
*   va_seq : Descriptif de l'etat journuit
*
*
* ARGUMENTS EN SORTIE :
*   pa_result : code de retour  XDC_OK ou XDC_NOK
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA_raf_SeqEch ";
   char vl_nom_var[256];
   char vl_val_text[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_raf_SeqEch" );
   
   /*A Affectation de la variable  */
   sprintf( vl_nom_var , "signale_%04d_scenario" ,va_etat_seq.NumEqt);
   
   /*A Recherche du lib du no de seq recupere */
   if ( MSRA_NomSequence( CM_SEQ_ECH,  va_etat_seq.SeqCourante , vl_val_text ) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de trouver la sequence no %d", va_etat_seq.SeqCourante );
      *pa_result=XDC_NOK;
      return ;
      }
          

   /*A Mise a jour de la vue */
   
   

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_raf_SeqEch" );
   *pa_result=XDC_OK;
   return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement de la vag affichant la couleur de la ZDP
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA16_raf_vag_ZDP(
          va_Mes,
          va_MesRADT,
          pa_result)
          
          XDY_EMes_TdP_6mn va_Mes;
          XDY_EMes_TdP_6mn va_MesRADT;
          int *pa_result;

/*
* ARGUMENTS EN ENTREE :
*   va_Mes : Descriptif de l'etat ZDP
*
*
* ARGUMENTS EN SORTIE :
*   pa_result : code de retour  XDC_OK ou XDC_NOK
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA16_raf_vag_ZDP ";
   char vl_nom_var[256];
   char vl_val_text[256];
   float vl_val_num;
   

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA16_raf_vag_ZDP" );
   
   /*A Affectation de la variable  */
   sprintf( vl_nom_var , "zdp_%04d" ,va_Mes.Numero);
   
          
   vl_val_num=(float) va_Mes.Temps;

   /*A Mise a jour de la vue */
   sprintf(vl_msg, "|10|105|%d|mesure|%f", va_Mes.Numero, vl_val_num);
   if (canWeSend(vl_msg)) {
	if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num, vl_nom_var );
		*pa_result = XDC_NOK;
		return;
	}
}
   

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA16_raf_vag_ZDP" );
   *pa_result=XDC_OK;
   return;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'une dispo d'un equipement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSRA14_raf_vag_DIS(
          va_typeeqt,
          va_numeqt,
          va_etat)
          
          XDY_TypeEqt va_typeeqt;
          XDY_Eqt     va_numeqt;
          XDY_Etat_Eqt va_etat;
          
          

/*
* ARGUMENTS EN ENTREE :
*    va_typeeqt: Type d'equipement a raf
*    va_numeqt : Numero de l'equipement
*    va_etat   : Etat de l'quipement
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA14_raf_vag_DIS ";
   char vl_nom_var[2256];
   char vl_nom_var2[256];
   char vl_pref_var[256];
   float vl_val_num=0;
   char *vl_val_text;
   int vl_etat4=0;
   int vl_indice=0;
   float vl_etat_bt=0;
   DONNEES *pl_liste;

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA14_raf_vag_DIS" );
   
   /*A Determination de l'etat de l'equipement : hs inhibe indispo marginal dispo */
    if ( (va_etat & XDC_EQT_HS) == XDC_EQT_HS )
       vl_etat4 = CM_HS ;
    else
	if ( (va_etat & XDC_EQT_INHIBE) == XDC_EQT_INHIBE )
		vl_etat4 = CM_INHIB;
	else
		if ( ((va_etat & XDC_EQT_CRITIQUE) == XDC_EQT_CRITIQUE) ||
				((va_etat & XDC_EQT_MAJEUR) == XDC_EQT_MAJEUR) ||
				((va_etat & XDC_EQT_DESACTIVE) == XDC_EQT_DESACTIVE) )
					vl_etat4 = CM_INDIS;
    	else
    		if ((va_etat & XDC_EQT_MINEUR) == XDC_EQT_MINEUR)
       			vl_etat4 = CM_MARG;
    		else
       			vl_etat4 = CM_DISPO;

   /*A Mise a jour de la variable */   
   vl_val_num = ((float) vl_etat4 / CM_NB_ETAT ) - ( 0.5 / CM_NB_ETAT );

	/*A Determination du nom de la vag de l'equipement a traiter */
	switch( va_typeeqt )
	{
		case XDC_EQT_NAV : /*B Cas particulier de la nav ou il faut etrre a jour 2 vag : biseaux + presignalisation */
		sprintf( vl_nom_var , "navb_%04d_etat" , va_numeqt);
		vl_val_num = ((float)vl_etat4 / CM_NB_ETAT ) - ( 0.5 / CM_NB_ETAT );

        sprintf(vl_msg,"|10|%d|%d|etatBiseau|%g", va_typeeqt, va_numeqt, vl_val_num);
		if (canWeSend(vl_msg)) 
		{
			if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num, vl_nom_var );
				return XDC_NOK;
			}
		}
        sprintf( vl_nom_var , "navp_%04d_etat" , va_numeqt);
        break;

      case XDC_EQT_SAGA:
		/*recherche du sous type*/
		for (pl_liste=pg_debut; pl_liste!=NULL; pl_liste = pl_liste->Suivant)
		{
		  if (pl_liste->Config.Numero == va_numeqt)
		    break;
		}
		if (pl_liste!=NULL) {
   	          sprintf(vl_msg,"|1|%d|%d|%g", pl_liste->Config.TypeSAGA, va_numeqt, vl_val_num);
   		  if (canWeSend(vl_msg)) 
			  	send_message(vl_msg, CM_BUFF_SOCK, "");
		}
		return (XDC_OK);
      		break;

      case XDC_EQT_PCT :
      case XDC_EQT_PMV :
/*         if ( ( va_typeeqt == XDC_EQT_PMV ) && 
              ( (vl_etat4 == CM_INDIS) || (vl_etat4 == CM_HS) || (vl_etat4 == CM_INHIB) )
            )
            {
            MSRA_RAZ_PMVPicto( (int) va_numeqt );
            }
*/
	/* PN : Inactivation PMV */
	/* La valeur etat est diff de la precedente , alors activer/inactiver le PMV */
   	if ( vl_val_num != ((float) vl_etat4 / CM_NB_ETAT ) - ( 0.5 / CM_NB_ETAT ) )
   	{
		if ( ( va_typeeqt == XDC_EQT_PMV ) && 
			( (vl_etat4 == CM_INDIS) || (vl_etat4 == CM_HS) || (vl_etat4 == CM_INHIB) ))
		            /*MSOP06_inactiver_PMV( (int) va_numeqt );*/
		{
            sprintf(vl_msg, "|10|2|%d|activation|0", va_numeqt);
			if (canWeSend(vl_msg)) 
			{
				if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
				{
					XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'inactiver le pmv %d" , va_numeqt);
					return XDC_NOK;
				}
			}
		}
		else
		{
			sprintf( vl_nom_var2 , "pmv_%04d_texte2" , va_numeqt);
   			/* S'il y a du texte sur le PMV et que ce n'est pas de l'heure alors activer */
   			if ( (strcmp(vl_val_text,"HEURE")) && (strcmp(vl_val_text,"")) )
            /* MSOP06_activer_PMV( (int) va_numeqt ); */
			{
            	sprintf(vl_msg, "|10|2|%d|activation|1", va_numeqt);
      	    	if (canWeSend(vl_msg)) 
				{
					if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
					{
						XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'activer le pmv %d" , va_numeqt);
						return XDC_NOK;
					}
				}
			}
		}
	}

	sprintf( vl_nom_var , "pmv_%04d_etat" , va_numeqt);

	break;

case XDC_EQT_ANA:
	sprintf( vl_nom_var , "dai_%04d_etat" , va_numeqt);
	break;
case XDC_EQT_CAM :
	sprintf( vl_nom_var , "camera_%04d_etat" , va_numeqt);
	break;
case XDC_EQT_PAU :
	sprintf( vl_nom_var , "pau_%04d_etat" , va_numeqt);
	break;
case XDC_EQT_RAD :
	/* Mise a jour pour radt*/
	sprintf( vl_nom_var , "radt_%04d_etat" , va_numeqt );
	break;
case XDC_EQT_LIT :
case XDC_EQT_EXT :
	sprintf( vl_nom_var , "tub_%04d_etat" , va_numeqt);
	break;
case XDC_EQT_ECHANGEUR :
	sprintf( vl_nom_var , "signalE_%04d_etat" , va_numeqt);
	break;
case XDC_EQT_TUBE :
	sprintf( vl_nom_var , "signal_%04d_etat" , va_numeqt);
	break;
case XDC_EQT_LTV :
	sprintf( vl_nom_var , "ltv_%04d_etat" , va_numeqt);
	break;
case XDC_EQT_PAN :
	sprintf( vl_nom_var , "fav_%04d_etat" , va_numeqt);
	break;
case XDC_EQT_ZDP :
	sprintf( vl_nom_var , "zdp_%04d_etat" , va_numeqt);
	break;
case XDC_EQT_PAL :
	sprintf( vl_nom_var , "pan_%04d_etat" , va_numeqt);
	break;
case XDC_EQT_PMVA :
	sprintf( vl_nom_var , "pmva_%04d_etat" , va_numeqt);
	if ( ( va_typeeqt == XDC_EQT_PMVA ) &&
			( (vl_etat4 == CM_INDIS) || (vl_etat4 == CM_HS) || (vl_etat4 == CM_INHIB) )
	)
	{
		MSRA_RAZ_PMVA( (int) va_numeqt );
	}
	break;
case XDC_EQT_PRV :
	sprintf( vl_nom_var , "prv_%04d_etat" , va_numeqt);
	/*    if ( ( va_typeeqt == XDC_EQT_PRV ) &&
              ( (vl_etat4 == CM_INDIS) || (vl_etat4 == CM_HS) || (vl_etat4 == CM_INHIB) )
            )
            {
            MSRA_RAZ_PRV( (int) va_numeqt );
            }*/
	break;
case XDC_EQT_PIC :
	sprintf (vl_nom_var, "pic_%04d_etat", va_numeqt);
	break;
case XDC_EQT_BAF :
	sprintf( vl_nom_var , "baf_%04d_etat" , va_numeqt);
	break;
case XDC_EQT_BAD :
	sprintf( vl_nom_var , "bad_%04d_etat" , va_numeqt);
	break;
case XDC_EQT_FAC :
case XDC_EQT_FAU :
	sprintf( vl_nom_var , "ferm_%04d_etat" , va_numeqt);
	break;
default  :
	XZST_03EcritureTrace(   XZSTC_WARNING , "MSRA14_raf_vag_DIS: Eqt %d de type %d non traite ds les synoptiques" , va_numeqt,va_typeeqt );
	return XDC_OK;
	break;
}
   /*vl_val_num = ((float) vl_etat4 / CM_NB_ETAT ) - ( 0.5 / CM_NB_ETAT );*/

   sprintf(vl_msg,"|1|%d|%d|%g", va_typeeqt, va_numeqt, vl_val_num);
   if (canWeSend(vl_msg)) 
	{
		if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num, vl_nom_var );
			return XDC_NOK;
		}
	}

   /*A Si l'etat est indispo alors on positionne le bouton etat au rouge */
   /*A Mise a jour de la vue */

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA14_raf_vag_DIS" );
   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'ALERTE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSRA15_raf_vag_ALERTE(
          pa_alerte)
          
          char *pa_alerte;
          
          

/*
* ARGUMENTS EN ENTREE :
*    pa_alerte : descriptif de l'alerte
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA15_raf_vag_ALERTE ";
 int vl_num_alerte;
 char vl_chaine_date[256];
 int vl_typeAlerte;
 char vl_nomType[256];
 int vl_typeEqt;
 int vl_numEqt;
 char vl_nom_Eqt[256];
 char vl_texte[256];
 int vl_autoroute;
 int vl_pr;
 int vl_sens;
 char vl_alerte[256];
 int vl_creation;
 char *pl_info;
 int vl_type_alerte;
 int vl_portion;
 int vl_translation;
 XDY_Evt vl_evt_vide;
 XDY_Voie	vl_voie;
 
   char vl_nom_var[256];
   char vl_pref_var[256];
   float vl_val_num=0;
   int vl_etat4=0;
   float vl_un=1;
   char vl_nom_objet[256];
   char vl_nom_objet_txt[256];
   DONNEES *pl_liste; 
   
   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSRA15_raf_vag_ALERTE '%s'",pa_alerte );
   
   strcpy( vl_alerte , pa_alerte );
   pl_info = strtok( vl_alerte , "|" );
   pl_info = strtok( NULL , "|" );
   pl_info = strtok( NULL , "|" );
   vl_type_alerte=atoi(pl_info);
   pl_info = strtok( NULL , "|" );
   pl_info = strtok( NULL , "|" );
   vl_typeEqt = atoi( pl_info );
   pl_info = strtok( NULL , "|" );
   vl_numEqt = atoi( pl_info );
   pl_info = strtok( NULL , "|" );
   pl_info = strtok( NULL , "|" );
   pl_info = strtok( NULL , "|" );
   pl_info = strtok( NULL , "|" );
   pl_info = strtok( NULL , "|" );
   vl_sens=atoi(pl_info);
   pl_info = strtok( NULL , "|" );
   vl_creation = atoi( pl_info );
   pl_info = strtok( NULL , "|" );
   XZST_03EcritureTrace(   XZSTC_WARNING, "IPHONE IN  : %d, %d",vl_sens,vl_creation);
 
   /*si alerte PAU virtuel*/
   if (vl_type_alerte==XZAAC_APPEL_IPHONE) {
     pl_info = strtok( NULL , "|" );
     vl_portion = atoi(pl_info);
     pl_info = strtok( NULL , "|" );
     vl_translation = atoi(pl_info);
     XZST_03EcritureTrace(   XZSTC_WARNING,"portion %d translation %d",vl_portion,vl_translation);

     /*on cree un objet mobile PAU VIRTUEL*/
     vl_evt_vide.numero=-1;
     vl_evt_vide.cle=-1;

     if (vl_sens==0)
       vl_voie = (XDY_Voie) 7; /*voie centrale*/ /*CM_LIGNE_TPC*/
     else
       vl_voie =  (XDY_Voie) 2 ; /*CM_LIGNE_BAU*/
	MSRA18_raf_objets_mobiles(XDC_SYN_IPHONE,
				vl_creation == XDC_NON ? XDC_SUPPR : XDC_AJOUT, /*typeraf*/
				vl_evt_vide,
				vl_evt_vide,
				vl_portion,
				vl_sens,
				vl_voie,
				vl_translation);
	/* double suppression car ce put... de tacli renvoie 2 fois la creation*/
	if (vl_creation == XDC_NON)
		MSRA18_raf_objets_mobiles(XDC_SYN_IPHONE,
                                vl_creation == XDC_NON ? XDC_SUPPR : XDC_AJOUT, /*typeraf*/
                                vl_evt_vide,
                                vl_evt_vide,
                                vl_portion,
                                vl_sens,
                                vl_voie,
                                vl_translation);
	
   return;
   }

   switch ( vl_typeEqt )
      {
      case XDC_EQT_DAI:
      case XDC_EQT_ANA:
         sprintf( vl_nom_var , "dai_%04d_alerte" , vl_numEqt);
         sprintf( vl_nom_objet , "dai_%04d" , vl_numEqt );
         sprintf( vl_nom_objet_txt , "tdai_%04d" , vl_numEqt );
         if ( vl_creation == XDC_NON ) 
            {
            vl_val_num = (float) 0.0;
             /*vl_tt = vm_tt_dai;
	     
             if ( vl_tt != NULL )
                MSVU_Ajout_Retire_Visi( vl_nom_var , vl_nom_objet_txt , "dai_visibilite" , &vl_tt );
             vl_tt = vm_tt_dai;
             if ( vl_tt != NULL )
                MSVU_Ajout_Retire_Visi( vl_nom_var , vl_nom_objet , "dai_visibilite" , &vl_tt );
	    */
            }
         else
            {
             vl_val_num = 1.0;
            /*A Inhibition du dyn de visi sur l'objet */
             /*vl_tt = NULL;
	     
             if ( MSVU_Ajout_Retire_Visi( vl_nom_var , vl_nom_objet , "dai_visibilite" , &vl_tt ) != XDC_OK )
                {
                XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de recuperer la tt de %s " , vl_nom_var );
                return;
                }
	    */
            /*vm_tt_dai = 1;
             vl_tt = NULL;
	     
             if ( MSVU_Ajout_Retire_Visi( vl_nom_var , vl_nom_objet_txt , "dai_visibilite" , &vl_tt ) != XDC_OK )
                {
                XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de recuperer la tt de %s " , vl_nom_var );
                return;
                }
	    
            vm_tt_dai = 1;*/
            }
	 /*
         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) &vl_val_num , V_F_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num , vl_nom_var );
            return XDC_NOK;
            }
	 */
	/*MSRA05_raf_vag_DAI_alerte(vl_numEqt,vl_creation);*/
   sprintf(vl_msg, "|10|4|%d|alerte|%d", vl_numEqt, vl_creation);
   if (canWeSend(vl_msg)) {
		if (send_message(vl_msg, CM_BUFF_SOCK,"") == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num, vl_nom_var );

			return;
		}
	}
         break;

      case XDC_EQT_SAGA:
	/*recherche du sous type*/
        for (pl_liste=pg_debut; pl_liste!=NULL; pl_liste = pl_liste->Suivant)
        {
             if (pl_liste->Config.Numero == vl_numEqt)
                      break;
        }
        if (pl_liste!=NULL) {
   	  sprintf(vl_msg, "|10|%d|%d|alerte|%d", pl_liste->Config.TypeSAGA,vl_numEqt,vl_creation);
	  if (canWeSend(vl_msg)) 
	    send_message(vl_msg, CM_BUFF_SOCK, "");
	}
      	break;

      case XDC_EQT_PAU :
         XZST_03EcritureTrace(   XZSTC_INFO, "MSRA15_raf_vag_ALERTE:* ON EST ANIME PAR EQUEXT DONC UN FAIT L'IMPASSE *");
         return XDC_OK;
         break;
      
      /** ON EST ANIME PAR EQUEXT DONC UN FAIT L'IMPASSE **/
      
      /*   sprintf( vl_nom_var , "pau_%04d_fonct" , vl_numEqt);
         sprintf( vl_nom_objet , "pau_%04d" , vl_numEqt );
         if ( vl_creation == XDC_NON ) 
            {
            vl_val_num = (float) 0.0;
             vl_tt = vm_tt_pau;
             if ( vl_tt != NULL )
                MSVU_Ajout_Retire_Visi( vl_nom_var , vl_nom_objet , "pau_visibilite" , &vl_tt );
            }
         else
            {
             vl_val_num = 0.37;
        */    /*A Inhibition du dyn de visi sur l'objet */
        /*     vl_tt = NULL;
             if ( MSVU_Ajout_Retire_Visi( vl_nom_var , vl_nom_objet , "pau_visibilite" , &vl_tt ) != XDC_OK )
                {
                XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de recuperer la tt de %s " , vl_nom_var );
                return;
                }
            vm_tt_pau = 1;
            }
         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) &vl_val_num , V_F_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num , vl_nom_var );
            return XDC_NOK;
            }
         break;
        */
         
      case XDC_EQT_EXT :
         sprintf( vl_nom_var , "tub_%04d_alerte" , vl_numEqt);
         vl_val_num = (float) vl_creation;
	 /*
         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) &vl_val_num , V_F_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num , vl_nom_var );
            return XDC_NOK;
            }
	 */
   sprintf(vl_msg, "|10|128|%d|alerte|%d", vl_numEqt, vl_creation);
   if (canWeSend(vl_msg)) {
		if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num, vl_nom_var );

			return;
		}
	}
         break;
      
      case XDC_EQT_LIT :
         sprintf( vl_nom_var , "tub_%04d_alerte" , vl_numEqt);
         sprintf( vl_nom_objet , "lit_%04d" , vl_numEqt );
         vl_val_num = (float) vl_creation;
         /*A Inhibition du dyn de visi sur l'objet */
	 /*
         if ( vl_creation == XDC_NON ) 
            {
            vl_tt = vm_tt_lit;
            if ( vl_tt != NULL )
               if ( MSVU_Ajout_Retire_Visi( vl_nom_var , vl_nom_objet , "lit_visibilite" , &vl_tt ) == XDC_NOK )
                  {
                  XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de trouver la tt de %s " , vl_nom_var );
                  return XDC_NOK;
                  }
            }
         else
            {
            vl_tt = NULL;
            MSVU_Ajout_Retire_Visi( vl_nom_var , vl_nom_objet , "lit_visibilite" , &vl_tt );
            vm_tt_lit = 1;
            }
     	
         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) &vl_val_num , V_F_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num , vl_nom_var );
            return XDC_NOK;
            }
	 */
   sprintf(vl_msg, "|10|10|%d|alerte|%d", vl_numEqt, vl_creation);
	if (canWeSend(vl_msg)) {
		if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_val_num, vl_nom_var );

			return;
		}
	}
         break;
      
      }
   
   /*A Mise a jour de la vue */

   

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA15_raf_vag_ALERTE" );
   return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Mise a zero du pmv et picto du numero donne
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA_RAZ_PMVPicto(
	va_no)
	
	int va_no;
          

/*
* ARGUMENTS EN ENTREE :
*   va_no : numero de l'eqt
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 : MSRA_RAZ_PMVPicto";
 XDY_Etat_PMV 	vl_etat_pmv;
 XDY_Etat_Picto vl_etat_picto;   
 int vl_result;
 
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_RAZ_PMVPicto");
   
   /*A Mise a zero de la structure etat pmv et picto pour le no d'eqt donne */
   vl_etat_pmv.Numero = va_no;
   vl_etat_pmv.Affichage1 = XDC_PMV_TEXTE;
   vl_etat_pmv.Affichage2 = XDC_PMV_TEXTE;
   vl_etat_pmv.Affichage3 = XDC_PMV_TEXTE;
   vl_etat_pmv.Affichage4 = XDC_PMV_TEXTE;
   
   strncpy( vl_etat_pmv.Texte1 , "                " ,XDC_Lon_Ligne_PMV-1 );
   strncpy( vl_etat_pmv.Texte2 , "                " ,XDC_Lon_Ligne_PMV-1 );
   strncpy( vl_etat_pmv.Texte3 , "                " ,XDC_Lon_Ligne_PMV-1 );
   strncpy( vl_etat_pmv.Texte4 , "                " ,XDC_Lon_Ligne_PMV-1 );
   
   vl_etat_pmv.Alternat1[0] = '\000';
   vl_etat_pmv.Alternat2[0] = '\000';
   vl_etat_pmv.Alternat3[0] = '\000';
   vl_etat_pmv.Alternat4[0] = '\000';
   
   vl_etat_pmv.Clign1 = XDC_PMV_PASCLIGNO;
   vl_etat_pmv.Clign2 = XDC_PMV_PASCLIGNO;
   vl_etat_pmv.Clign3 = XDC_PMV_PASCLIGNO;
   vl_etat_pmv.Flash = XDC_PMV_PASFLASH;
   
   vl_etat_picto.Numero = va_no ;
   strcpy( vl_etat_picto.Picto, "XXXX" );
   strncpy( vl_etat_picto.Texte, "               " , XDC_Lon_TextePicto-1 );
   
   
   /*A Appel des fct de raf des pmv et picto */
   MSRA01_raf_vag_PMV_Message( vl_etat_pmv, &vl_result);
   MSRA02_raf_vag_PMV_Picto( vl_etat_picto, &vl_result);
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_RAZ_PMVPicto");
   return XDC_OK  ;
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

float MSRA_No_Picto(
          pa_nom_picto)
          
          char *pa_nom_picto;

/*
* ARGUMENTS EN ENTREE :
*   pa_nom_picto : Nom du picto a traiter
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA_No_Picto ";
   int vl_icode_retour = 1; /* indice et code retour */
   float vl_code_retour;
   
   XZST_03EcritureTrace(   XZSTC_INTERFACE, "IN  : MSRA_No_Picto pa_nom_picto:[%s]" , pa_nom_picto);
   
   /* Recherche le nom passe en parametre dans le tableau des parametre des objets  et retour l'indice du tableau */
   while ( (vl_icode_retour <= CM_NB_PICTO) && ( strcmp ( vm_table_picto[vl_icode_retour] , pa_nom_picto)) )
      {
//   XZST_03EcritureTrace(   XZSTC_WARNING, "code picto=[%s] numero=[%d]", vm_table_picto[vl_icode_retour],vl_icode_retour);
         vl_icode_retour++;
      }
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_No_Picto retour %f" , (float)vl_icode_retour  );
   return ((float)vl_icode_retour);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

float MSRA_No_Picto_SAGA(
          pa_nom_picto)
          
          char *pa_nom_picto;

/*
* ARGUMENTS EN ENTREE :
*   pa_nom_picto : Nom du picto a traiter
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA_No_Picto ";
   int vl_icode_retour = CM_NB_PICTO; /* indice et code retour */
   float vl_code_retour;
   
   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSRA_No_Picto *pa_nom_picto:%s" , pa_nom_picto);
   
   /* Recherche le nom passe en parametre dans le tableau des parametre des objets  et retour l'indice du tableau */
   while ( (vl_icode_retour > 0) && ( strcmp ( vm_table_picto_SAGA[vl_icode_retour] , pa_nom_picto)) )
      {
      vl_icode_retour--;
      }
/* REPERE1 */
   return ((float)vl_icode_retour);
   vl_code_retour = ((float)( (float)(vl_icode_retour)-0.5)/ (float)(CM_NB_PICTO+1) ) ;
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_No_Picto_SAGA retour %f" , vl_code_retour  );
   return vl_code_retour  ;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

float MSRA_No_Panneau(
          pa_nom_picto)
          
          char *pa_nom_picto;

/*
* ARGUMENTS EN ENTREE :
*   pa_nom_picto : Nom du picto a traiter
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA_No_Panneau ";
   int vl_icode_retour = CM_NB_PANNEAU; /* indice et code retour */
   float vl_code_retour;
   
   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSRA_No_Panneau *pa_nom_picto:%s" , pa_nom_picto);
   
   /* Recherche le nom passe en parametre dans le tableau des parametre des objets  et retour l'indice du tableau */
   while ( (vl_icode_retour > 0) && ( strcmp ( vm_table_picto[vl_icode_retour] , pa_nom_picto) ) )
      {
      vl_icode_retour--;
      }
   vl_code_retour = ((float)( vl_icode_retour-1)/ (CM_NB_PANNEAU+1) ) + CM_DELTA_PANNEAU ;
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_No_Panneau retour %f" , vl_code_retour  );
   return vl_code_retour  ;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*        Renvoie le libelle du no de seq donne 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA_NomSequence(
          va_type_seq,
          va_no_seq,
          pa_seq)
          
          XDY_Octet va_type_seq;
          XDY_Octet va_no_seq;
          char *pa_seq;
          
/*
* ARGUMENTS EN ENTREE :
*          va_type_seq : type de seq demande : CM_SEQ_ECH ou CM_SEQ_TUN
*          va_no_seq : numero de sequence donne
*   
*
*
* ARGUMENTS EN SORTIE :
*          pa_seq : Libelle de sequecnec
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 : MSRA_NomSequence ";
   float vl_code_retour;
   char vl_nom_fic[256];
   FILE *pl_fic_seq;
   char vl_chaine[256];
   int vl_no=-1;
   char *pl_info;
   int i=0;
   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_NomSequence type seq %d  no_seq %d" , va_type_seq, va_no_seq);

   /*A Refus d'un no de seq a 0 */
   if ( va_no_seq == 0)
      {
      XZST_03EcritureTrace(   XZSTC_INFO, " Numero de sequence egale a 0" );
      return XDC_NOK;
      }
   
   /*A Detreminer le fichier a lire a partir du type de seq */
   switch( va_type_seq )
      {
      case CM_SEQ_ECH :
         strcpy( vl_nom_fic , CM_FIC_SEQ_ECH );
         break;
      case CM_SEQ_TUN :
         strcpy( vl_nom_fic , CM_FIC_SEQ_TUN );
         break;
      default :
         XZST_03EcritureTrace(   XZSTC_WARNING , "Type de seq inconnu : %d", va_type_seq );
         return XDC_NOK;
      }
   
   /*A Ouverture du ficher de sequences */
   if ( ( pl_fic_seq = fopen( vl_nom_fic , "r")) == NULL ) 
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, "Fichier contenant les sequences introuvable : %s (errno=%d)", vl_nom_fic , errno);
      return XDC_NOK;
      }
   
   /*A Recherche dans le fichier du no de seq */
   while ( (fgets( vl_chaine, 256 ,pl_fic_seq)  != NULL) && (  ( vl_no = atoi( vl_chaine )) != va_no_seq ) ); /*!*/
     
   /*A A t_on trouve le no de seq , Hein ? */
   if ( vl_no != va_no_seq )
      {
      /*B Non, et c'est pas bien */
      XZST_03EcritureTrace(   XZSTC_WARNING, " Numero de sequence %d introuvable dans le fichier %s.", va_no_seq , vl_nom_fic);
      fclose( pl_fic_seq );
      return XDC_NOK;
      }
   
    fclose( pl_fic_seq );
   
    /*A Recuperation du libelle */
    pl_info = strtok( vl_chaine , CM_SEPAR );
    pl_info = strtok( NULL , CM_SEPAR );

    
    /*A Nettoyage des blancs et tab */
    for (; (( *pl_info != '\000')) && 
              ( ( *pl_info == ' ' ) || ( *pl_info == '\t' ) )
            ; pl_info++ );
            
    for (i=strlen( pl_info )-1; ( i > 0 ) &&
                              ( ( pl_info[i] == ' ' ) || ( pl_info[i] == '\t' ) )
                            ; i--);

    if ( (i > 0) && (i < strlen( pl_info )-1 ) )
       {
       if ( ( pl_info[i] == ' ' ) || ( pl_info[i] == '\t' ) )
          pl_info= '\000';
       else
          pl_info[i]= '\000';
       }
       
    strcpy( pa_seq , pl_info );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_NomSequence retour XDC_OK"   );
   return XDC_OK  ;
}


/*PMVA DEBUT*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un PMVA 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA12_raf_vag_PMVA(
          va_etat_pmv,
          pa_result, 
          va_machine)
          
          XDY_Etat_PMVA va_etat_pmv;
          int *pa_result;
          char *va_machine;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_pmv : Descriptif de l'etat du pmv a rafraichir
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
   char vl_nom_var[256];
   char vl_pref_var[256];
   float vl_val_num;
   int vl_activer=1;

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA12_raf_vag_PMVA" );

   /*A  Construction du prefise des variables associees au pmv */
   sprintf( vl_pref_var , "pmva_%04d_" , va_etat_pmv.Numero );

   va_etat_pmv.Texte1[XDC_Lon_Ligne_PMVA-1]='\000';
   va_etat_pmv.Texte2[XDC_Lon_Ligne_PMVA-1]='\000';
   va_etat_pmv.Texte3[XDC_Lon_Ligne_PMVA-1]='\000';
   va_etat_pmv.Texte4[XDC_Lon_Ligne_PMVA-1]='\000';
   va_etat_pmv.Texte5[XDC_Lon_Ligne_PMVA-1]='\000';

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d %d %d %d %d",
			va_etat_pmv.Affichage1,
			va_etat_pmv.Affichage2,
			va_etat_pmv.Affichage3,
			va_etat_pmv.Affichage4,
			va_etat_pmv.Affichage5);
   XZST_03EcritureTrace(   XZSTC_DEBUG1," %s(%d) %s(%d) %s(%d) %s(%d) %s(%d)",
			va_etat_pmv.Texte1,
			strlen(va_etat_pmv.Texte1),
			va_etat_pmv.Texte2,
			strlen(va_etat_pmv.Texte2),
			va_etat_pmv.Texte3,
			strlen(va_etat_pmv.Texte3),
			va_etat_pmv.Texte4,
			strlen(va_etat_pmv.Texte4),
			va_etat_pmv.Texte5,
			strlen(va_etat_pmv.Texte5));
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "FLASH = %d",va_etat_pmv.Flash);
   /*A Affectation de la variable pmx_xxxx_texte1 */
   sprintf( vl_nom_var , "%stexte1" , vl_pref_var);
   vl_activer=0;
   switch ( va_etat_pmv.Affichage1 )
      {
      case XDC_PMVA_TEXTE :
	 if (strlen(va_etat_pmv.Texte1) > 0)
           vl_activer=1;

	sprintf(vl_msg, "|10|46|%d|texte1|%s", va_etat_pmv.Numero, va_etat_pmv.Texte1);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag texte1" , va_etat_pmv.Texte1 );
				*pa_result = XDC_NOK;
				return;
			}
		}
	 /*
         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) va_etat_pmv.Texte1 , V_T_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , va_etat_pmv.Texte1 , vl_nom_var );
            *pa_result = XDC_NOK;
            return;
            }
	 */
         break;
      case XDC_PMVA_TEMPERATURE :
         vl_activer=0;

	sprintf(vl_msg, "|10|46|%d|texte1|  ", va_etat_pmv.Numero);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag texte1" , va_etat_pmv.Texte1 );
				*pa_result = XDC_NOK;
				return;
			}
		}

	 /*
         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) "  " , V_T_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte TEMPERATURE a la vag %s"  , vl_nom_var );
            *pa_result = XDC_NOK;
            return;
            }
	 */
         break;
      case XDC_PMVA_HEURE :
         vl_activer=0;
        sprintf(vl_msg, "|10|46|%d|texte1|  ", va_etat_pmv.Numero);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag texte1" , "   " );
				*pa_result = XDC_NOK;
				return;
			}
		}
	 /*
         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) "  " , V_T_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte HEURE a la vag %s" , vl_nom_var );
            *pa_result = XDC_NOK;
            return;
            }
	 */
         break;
      default :
         XZST_03EcritureTrace(   XZSTC_WARNING , "Type d'affichage 1 PMV invalide  %d" ,  va_etat_pmv.Affichage1 );
         *pa_result = XDC_NOK;
            return;

      }

   /*A Affectation de la variable pmx_xxxx_texte2 */
   sprintf( vl_nom_var , "%stexte2" , vl_pref_var);
   switch ( va_etat_pmv.Affichage2 )
      {
      case XDC_PMVA_TEXTE :
	 if (strlen(va_etat_pmv.Texte2) > 0)
           vl_activer=1;
	
	sprintf(vl_msg, "|10|46|%d|texte2|%s", va_etat_pmv.Numero, va_etat_pmv.Texte2);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag texte1" , va_etat_pmv.Texte2 );
				*pa_result = XDC_NOK;
				return;
			}
		}
	 /*
         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) va_etat_pmv.Texte2 , V_T_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , va_etat_pmv.Texte2 , vl_nom_var );
            *pa_result = XDC_NOK;
            return;
            }
	 */
         break;
      case XDC_PMVA_TEMPERATURE :
         vl_activer=0;
	sprintf(vl_msg, "|10|46|%d|texte2|  ", va_etat_pmv.Numero);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag texte2" , "  ");
				*pa_result = XDC_NOK;
				return;
			}
		}
	 /*

         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) "TEMPERATURE" , V_T_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte TEMPERATURE a la vag %s"  , vl_nom_var );
            *pa_result = XDC_NOK;
            return;
            }
	 */
         break;
      case XDC_PMVA_HEURE :
         vl_activer=0;

	sprintf(vl_msg, "|10|46|%d|texte2|  ", va_etat_pmv.Numero);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag texte2" , "  ");
				*pa_result = XDC_NOK;
				return;
			}
		}
	 /*
         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) "HEURE" , V_T_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte HEURE a la vag %s" , vl_nom_var );
            *pa_result = XDC_NOK;
            return;
            }
	 */
         break;
      default :
         XZST_03EcritureTrace(   XZSTC_WARNING , "Type d'affichage 2 PMV invalide  %d" ,  va_etat_pmv.Affichage2 );
         *pa_result = XDC_NOK;
            return;

      }
   /*A Affectation de la variable pmx_xxxx_texte3 */
   sprintf( vl_nom_var , "%stexte3" , vl_pref_var);
   switch ( va_etat_pmv.Affichage3 )
      {
      case XDC_PMVA_TEXTE :
	 if (strlen(va_etat_pmv.Texte3) > 0)
           vl_activer=1;

	sprintf(vl_msg, "|10|46|%d|texte3|%s", va_etat_pmv.Numero, va_etat_pmv.Texte3);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag texte3" , va_etat_pmv.Texte3 );
				*pa_result = XDC_NOK;
				return;
			}
		}
	 /*
         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) va_etat_pmv.Texte3 , V_T_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , va_etat_pmv.Texte3 , vl_nom_var );
            *pa_result = XDC_NOK;
            return;
            }
	 */
         break;
      case XDC_PMVA_TEMPERATURE :
         vl_activer=0;

	sprintf(vl_msg, "|10|46|%d|texte3|  ", va_etat_pmv.Numero);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag texte3" , "  ");
				*pa_result = XDC_NOK;
				return;
			}
		}
	 /*
         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) " " , V_T_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte TEMPERATURE a la vag %s"  , vl_nom_var );
            *pa_result = XDC_NOK;
            return;
            }
	 */
         break;
      case XDC_PMVA_HEURE :
         vl_activer=0;

	sprintf(vl_msg, "|10|46|%d|texte3|HEURE", va_etat_pmv.Numero);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag texte3" , "HEURE");
				*pa_result = XDC_NOK;
				return;
			}
		}
	 /*
         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) " " , V_T_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte HEURE a la vag %s" , vl_nom_var );
            *pa_result = XDC_NOK;
            return;
            }
	 */
         break;
      default :
         XZST_03EcritureTrace(   XZSTC_WARNING , "Type d'affichage 3 PMV invalide  %d" ,  va_etat_pmv.Affichage3 );
         *pa_result = XDC_NOK;

      }
   /*A Affectation de la variable pmx_xxxx_texte4 */
   sprintf( vl_nom_var , "%stexte4" , vl_pref_var);
   switch ( va_etat_pmv.Affichage4 )
      {
      case XDC_PMVA_TEXTE :
	 if (strlen(va_etat_pmv.Texte4) > 0)
           vl_activer=1;

	sprintf(vl_msg, "|10|46|%d|texte4|%s", va_etat_pmv.Numero, va_etat_pmv.Texte4);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag texte4" , va_etat_pmv.Texte4 );
				*pa_result = XDC_NOK;
				return;
			}
		}
		/* 

         if ( MSVU_Affecte_vag( vl_nom_var , (ADDRESS) va_etat_pmv.Texte4 , V_T_TYPE ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag %s" , va_etat_pmv.Texte4 , vl_nom_var );
            *pa_result = XDC_NOK;
            return;
            }
	 */
         break;
      case XDC_PMVA_TEMPERATURE :
         vl_activer=0;
	sprintf(vl_msg, "|10|46|%d|texte4|  ", va_etat_pmv.Numero);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag texte4" , "  ");
				*pa_result = XDC_NOK;
				return;
			}
		}
         break;
      case XDC_PMVA_HEURE :
         vl_activer=0;
	
	sprintf(vl_msg, "|10|46|%d|texte4|  ", va_etat_pmv.Numero);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag texte4" , "  ");
				*pa_result = XDC_NOK;
				return;
			}
		}
         break;
      default :
         XZST_03EcritureTrace(   XZSTC_WARNING , "Type d'affichage 3 PMV invalide  %d" ,  va_etat_pmv.Affichage4 );
         *pa_result = XDC_NOK;

      }
   /*A Affectation de la variable pmx_xxxx_texte3 */
   sprintf( vl_nom_var , "%stexte5" , vl_pref_var);
   switch ( va_etat_pmv.Affichage5 )
      {
      case XDC_PMVA_TEXTE :
	 if (strlen(va_etat_pmv.Texte5) > 0)
           vl_activer=1;
	
	sprintf(vl_msg, "|10|46|%d|texte5|%s", va_etat_pmv.Numero, va_etat_pmv.Texte5);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag texte5" , va_etat_pmv.Texte5 );
				*pa_result = XDC_NOK;
				return;
			}
		}
         break;
      case XDC_PMVA_TEMPERATURE :
         vl_activer=0;

	sprintf(vl_msg, "|10|46|%d|texte5|  ", va_etat_pmv.Numero);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag texte5" , "  ");
				*pa_result = XDC_NOK;
				return;
			}
		}
         break;
      case XDC_PMVA_HEURE :
         vl_activer=0;
	sprintf(vl_msg, "|10|46|%d|texte5|  ", va_etat_pmv.Numero);
   	if (canWeSend(vl_msg)) {
			if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %s a la vag texte5" , "  ");
				*pa_result = XDC_NOK;
				return;
			}
		}
         break;
      default :
         XZST_03EcritureTrace(   XZSTC_WARNING , "Type d'affichage 3 PMV invalide  %d" ,  va_etat_pmv.Affichage5 );
         *pa_result = XDC_NOK;

      } 
   sprintf( vl_nom_var , "%smodule" , vl_pref_var);
   vl_val_num = (float) vl_activer;
   /* Affectation de la variable pmx_xxxx_cligno1 */
   sprintf( vl_nom_var , "%scligno1" , vl_pref_var);
   if ( va_etat_pmv.Clign1 != XDC_PMVA_PASCLIGNO)
      vl_val_num = (float) 1;
   else
      vl_val_num = (float) 0;
   /* Affectation de la variable pmx_xxxx_cligno2 */
   sprintf( vl_nom_var , "%scligno2" , vl_pref_var);
   if ( va_etat_pmv.Clign2 != XDC_PMVA_PASCLIGNO )
      vl_val_num = (float) 1;
   else
      vl_val_num = (float) 0;
   /* Affectation de la variable pmx_xxxx_cligno3 */
   sprintf( vl_nom_var , "%scligno3" , vl_pref_var);
   if ( va_etat_pmv.Clign3 != XDC_PMVA_PASCLIGNO )
      vl_val_num = (float) 1;
   else
      vl_val_num = (float) 0;

   /* Affectation de la variable pmx_xxxx_cligno4 */
   sprintf( vl_nom_var , "%scligno4" , vl_pref_var);
   if ( va_etat_pmv.Clign4 != XDC_PMVA_PASCLIGNO )
      vl_val_num = (float) 1;
   else
      vl_val_num = (float) 0;
   /* Affectation de la variable pmx_xxxx_cligno5 */
   sprintf( vl_nom_var , "%scligno5" , vl_pref_var);
   if ( va_etat_pmv.Clign5 != XDC_PMVA_PASCLIGNO )
      vl_val_num = (float) 1;
   else
      vl_val_num = (float) 0;
   /* Affectation de la variable pmx_xxxx_flash */
   sprintf( vl_nom_var , "%sflash" , vl_pref_var);
   if ( va_etat_pmv.Flash == XDC_PMVA_FLASH )
      vl_val_num = (float) 1;
   else
      vl_val_num = (float) 0;
      
    sprintf(vl_msg, "|10|46|%d|flash|%d", va_etat_pmv.Numero, vl_val_num);
   	if (canWeSend(vl_msg)) {
		if (send_message(vl_msg, CM_BUFF_SOCK, va_machine) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag flash" , vl_val_num);
			*pa_result = XDC_NOK;
			return;
		}
	}
   /* Affectation de la variable pmx_xxxx_alternat , s'il y a une ligne alterna alors il ya alternat*/
   sprintf( vl_nom_var , "%salternat" , vl_pref_var);
   if ( ( va_etat_pmv.Alternat1[0] != '\000' ) ||
        ( va_etat_pmv.Alternat2[0] != '\000' ) ||
        ( va_etat_pmv.Alternat3[0] != '\000' ) ||
        ( va_etat_pmv.Alternat5[0] != '\000' ) ||
        ( va_etat_pmv.Alternat4[0] != '\000' ) )
      {
      vl_val_num = (float) 1;
      }
   else
      {
      vl_val_num = (float) 0;
      }
  
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA12_raf_vag_PMVA_Message" );
   *pa_result = XDC_OK;
  
   return;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Mise a zero du pmvA du numero donne
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA_RAZ_PMVA(
        va_no)

        int va_no;


/*
* ARGUMENTS EN ENTREE :
*   va_no : numero de l'eqt
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*  
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 1.44 03/26/07 : MSRA_RAZ_PMVA";
 XDY_Etat_PMVA  vl_etat_pmv;
 int vl_result;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_RAZ_PMVA");
  
   /*A Mise a zero de la structure etat pmv pour le no d'eqt donne */
   vl_etat_pmv.Numero = va_no;
   vl_etat_pmv.Affichage1 = XDC_PMVA_TEXTE;
   vl_etat_pmv.Affichage2 = XDC_PMVA_TEXTE;
   vl_etat_pmv.Affichage3 = XDC_PMVA_TEXTE;
   vl_etat_pmv.Affichage4 = XDC_PMVA_TEXTE;
   vl_etat_pmv.Affichage5 = XDC_PMVA_TEXTE;
  
   strncpy( vl_etat_pmv.Texte1 , "                " ,XDC_Lon_Ligne_PMVA-1 );
   strncpy( vl_etat_pmv.Texte2 , "                " ,XDC_Lon_Ligne_PMVA-1 );
   strncpy( vl_etat_pmv.Texte3 , "                " ,XDC_Lon_Ligne_PMVA-1 );
   strncpy( vl_etat_pmv.Texte4 , "                " ,XDC_Lon_Ligne_PMVA-1 );
   strncpy( vl_etat_pmv.Texte5 , "                " ,XDC_Lon_Ligne_PMVA-1 );
  
   vl_etat_pmv.Alternat1[0] = '\000';
   vl_etat_pmv.Alternat2[0] = '\000';
   vl_etat_pmv.Alternat3[0] = '\000';
   vl_etat_pmv.Alternat4[0] = '\000';
   vl_etat_pmv.Alternat5[0] = '\000';
  
   vl_etat_pmv.Clign1 = XDC_PMVA_PASCLIGNO;
   vl_etat_pmv.Clign2 = XDC_PMVA_PASCLIGNO;
   vl_etat_pmv.Clign3 = XDC_PMVA_PASCLIGNO;
   vl_etat_pmv.Clign4 = XDC_PMVA_PASCLIGNO;
   vl_etat_pmv.Clign5 = XDC_PMVA_PASCLIGNO;
   vl_etat_pmv.Flash = XDC_PMVA_PASFLASH;


   /*A Appel des fct de raf des pmv et picto */
   MSRA12_raf_vag_PMVA( vl_etat_pmv, &vl_result, "");
  

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_RAZ_PMVA");
   return XDC_OK  ;
}

/*SDU*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Masque/Affiche les equipements
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA18_Masque(
          va_type_eq)

		  XDY_TypeEqt va_type_eq;

/*
* ARGUMENTS EN ENTREE :
*   va_type_eq : Type d'equipement a Masquer/Afficher
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA18_Masque ";
   float vl_val_num;
   char vl_msg[20];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA18_Masque" );

   if (va_type_eq == 5) /*commuter*/
   sprintf (vl_msg, "|%d|0|",CMD_FLASH_COMMUTER);
   else if (va_type_eq ==155) /*projeter*/
     sprintf(vl_msg, "|%d|%d|",  va_type_eq,0);
   else
   sprintf(vl_msg, "|%d|%d|", CMD_FLASH_MASQUE, va_type_eq);
   if (send_message(vl_msg, 20, "") == -1)
       {
       XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le masque %d" , va_type_eq);
       return XDC_NOK;
       }

   /*A Mise a jour de la vue */

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA18_Masque" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Masque/Affiche une vue
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA19_Vue(
          va_nom_vue)

		  XDY_Nom va_nom_vue;

/*
* ARGUMENTS EN ENTREE :
*   va_nom_vue : nom de la vue
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA19_Vue ";
   float vl_val_num;

   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSRA19_Vue %s",va_nom_vue );

   /*si le nom de la vue commence par '|'*/
   /* LCL : On ne tronque pas les chaines */
   /*va_nom_vue[XDC_Lon_Nom-1]='\000';*/
   if (va_nom_vue[0]=='|') 
     sprintf(vl_msg,"|%d%s|",CMD_FLASH_TRONCON,va_nom_vue);
   else
     sprintf(vl_msg, "|%d|%s|", CMD_FLASH_VUE, va_nom_vue);
   if (send_message(vl_msg, CM_BUFF_SOCK,"") == -1)
       {
       XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer l'affichage de la vue %s" , va_nom_vue);
       return XDC_NOK;
       }

   /*A Mise a jour de la vue */
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA19_Vue" );
   return XDC_OK;
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Masque/Affiche une vue
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA20_TdpMZ(
          va_chaine)

		  char *va_chaine;

/*
* ARGUMENTS EN ENTREE :
*   va_nom_vue : nom de la vue
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA20_TdpMZ ";
   float vl_val_num;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA20_TdpMZ" );

   if (MSCO04_init_courbe_tdp_mz(va_chaine) == XDC_NOK)
   {
       XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'initialiser l'affichage de la courbe %s" , va_chaine);
       return XDC_NOK;
   }

   /*A Mise a jour de la vue */
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA20_TdpMZ" );
   return XDC_OK;
}

int MSRA21_PlanSecours(
          va_num_plan)

	XDY_Entier va_num_plan;

/*
* ARGUMENTS EN ENTREE :
*   va_num_plan : Numero de plan a visualiser
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA21_PlanSecours ";
   float vl_val_num;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA21_PlanSecours" );

   sprintf(vl_msg, "|%d|%d|", CMD_FLASH_PLAN_SECOURS, va_num_plan);
   if (send_message(vl_msg, CM_BUFF_SOCK, "") == -1)
       {
       XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le plan de secours %d" , va_num_plan);
       return XDC_NOK;
       }

   /*A Mise a jour de la vue */

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA21_PlanSecours" );
   return XDC_OK;
}


int send_message(message, lg_buf, machine)
char *message;
int lg_buf;
char *machine;
{
   int lg, res;
   int vl_indice=0;
   char *vl_msg2;
   XDY_Datagroup vl_dg;   
   XDY_NomMachine vl_machine;

   if (!vg_pas_tisse) {
      if (!strcmp(machine,""))
         strcpy(vl_machine, vm_NomSite);
      else
      {
         if (!strcmp(machine,XDC_NOM_SITE_VC) || !strcmp(machine,XDC_NOM_SITE_DP))
            if (!strcmp(vm_NomSite, XDC_NOM_SITE_CI))
               strcpy(machine,XDC_NOM_SITE_CI);

         strcpy(vl_machine, machine);
      }

     XDG_EncodeDG2(vl_dg, XDG_API, vm_NomSite);
     TipcSrvMsgWrite(vl_dg,
               XDM_IdentMsg(XDM_API_SYNOS),
               XDM_FLG_SRVMSGWRITE,
               T_IPC_FT_STR, message,
	       T_IPC_FT_STR, vl_machine,
	       NULL);
     return (XDC_OK);
   }

   if (vg_tracefile!=NULL) {
   fprintf(vg_tracefile,"%s\n",message);
   fflush(vg_tracefile);
   }
   if ((vl_msg2 = malloc(3+strlen(message)+1)) == NULL)
      return -1;
   if (strncmp(message,"|", 1) == 0)
   {
	   lg = strlen(message) - 1;
	   sprintf(vl_msg2,"%03d%s",lg,message+1);
   }
   else{
	   lg = strlen(message);
	   sprintf(vl_msg2,"%03d%s",lg,message);
   }

   if (vm_taille_emise_normal > CM_LIMITE_EMISSION_SOCKET){
		usleep(200000);
		vm_taille_emise_normal = 0;
	}


   //   sprintf(vl_msg2,"%03d%s",lg,message+1);
//   sem_wait(&vg_semaphore);
   res = send(vg_cli_sock_ToFlash, vl_msg2,strlen(vl_msg2), 0);
//   sem_post(&vg_semaphore);
   XZST_03EcritureTrace(XZSTC_DEBUG1," send = %d(%s)", res, vl_msg2);

   if (res<=0)
	   XZST_03EcritureTrace(XZSTC_WARNING," ###### ERREUR SEND %d #######", res);
   else
	   vm_taille_emise_normal += res;


   return res;
}

int send_message_courbe(message, lg_buf)
char *message;
int lg_buf;
{
   int lg, res;
   int vl_indice=0;
   char *vl_msg2;
   XDY_Datagroup vl_dg;

   if (!vg_pas_tisse) {
     XDG_EncodeDG2(vl_dg, XDG_API, vm_NomSite);
     TipcSrvMsgWrite(vl_dg,
               XDM_IdentMsg(XDM_API_SYNOS),
               XDM_FLG_SRVMSGWRITE,
               T_IPC_FT_STR, message,
	            T_IPC_FT_STR, vm_NomSite,
	       NULL);
     return (XDC_OK);
   }

   if ((vl_msg2 = malloc(3+strlen(message)+1)) == NULL)
      return -1;
   if (strncmp(message,"|", 1) == 0)
   {
	   lg = strlen(message) - 1;
	   sprintf(vl_msg2,"%03d%s",lg,message+1);
   }
   else{
	   lg = strlen(message);
	   sprintf(vl_msg2,"%03d%s",lg,message);
   }

   if (vm_taille_emise_courbes > CM_LIMITE_EMISSION_SOCKET){
	usleep(200000);
   	vm_taille_emise_courbes = 0;
   }

   //sem_wait(&vg_semaphore);
   res = send(vg_cli_sock_Courbes, vl_msg2,strlen(vl_msg2), 0);
   //sem_post(&vg_semaphore);
   XZST_03EcritureTrace(XZSTC_WARNING," send = %d(%s)", res, vl_msg2);

   if (res<=0)
	   XZST_03EcritureTrace(XZSTC_WARNING," ###### ERREUR SEND %d #######", res);
   else
	   vm_taille_emise_courbes += res;
      /*
   res = send(vg_cli_sock_ToFlash, vl_msg2, lg_buf, 0);

   res = send(vg_cli_sock_ToFlash, message,strlen(message), 0);*/


	return res;
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *   Mise a zero du prv du numero donne
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int MSRA_RAZ_PRV(
		va_no)

int va_no;


/*
 * ARGUMENTS EN ENTREE :
 *   va_no : numero de l'eqt
 *
 *
 * ARGUMENTS EN SORTIE :
 *   < Valeur et descriptif des parametres en sortie >
 *
 *
 * CODE RETOUR :
 *   XDC_OK
 *   XDC_NOK
 *
 * CONDITION D'UTILISATION
 *   < Explication succinte sur les conditions  d'utilisation
 *     de la fonction
 *
 * FONCTION
 *
 *
------------------------------------------------------*/
{static char *version = "@(#)isyn_sra_rafobj.c	1.44 1.44 03/26/07 : MSRA_RAZ_PRV";
XDY_Etat_PRV  vl_etat_prv;
int vl_result;

XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_RAZ_PRV");

/*A Mise a zero de la structure etat pmv pour le no d'eqt donne */
vl_etat_prv.Numero = va_no;

strncpy( vl_etat_prv.Vitesse , "                " ,XDC_Lon_Ligne_PRV-1 );
strncpy( vl_etat_prv.Bandeau , "                " ,XDC_Lon_Ligne_PRV-1 );

/*A Appel des fct de raf des pmv et picto */
MSRA15_raf_vag_PRV( vl_etat_prv, &vl_result);


XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_RAZ_PRV");
return XDC_OK  ;
}



/*SDU*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Masque/Affiche les equipements
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA18_Masque_Projeter(
          va_type_eq,va_etat)

		  XDY_TypeEqt va_type_eq;
		  XDY_TypeEqt va_etat;

/*
* ARGUMENTS EN ENTREE :
*   va_type_eq : Type d'equipement a Masquer/Afficher
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_rafobj.c	1.44 03/26/07 :MSRA18_Masque ";
   float vl_val_num;
   char vl_msg[20];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA18_Masque_Projeter" );

   sprintf(vl_msg, "|%d|%d|%d", CMD_FLASH_PROJETER, va_type_eq,va_etat);
   if (send_message(vl_msg, 20,"") == -1)
       {
       XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le masque %d" , va_type_eq);
       return XDC_NOK;
       }

   /*A Mise a jour de la vue */

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA18_Masque_Projeter" );
   return XDC_OK;
}
