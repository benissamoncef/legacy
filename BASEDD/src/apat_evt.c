/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : apat_evt.c				*
*************************************************
* DESCRIPTION :
* module externe de la tache passerelle generique
* 
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
* ABE	22/07/2020	Creation DEM SAE149 V1.1
* ABE	28/07/2020	Correction PR en amont pour sens 2 DEM-SAE149 V1.2
* ABE	29/07/2020	Correction PR en amont pour sens 2 seulement si le PR n'est pas rond DEM-SAE149 V1.3
*************************************************/

#include "apat_evt.h"
#include "xzae198.h"	
#include "xzae197.h"
#include "xzlg.h"

void apat_envoie_pat_en_cours()
{
 	char       va_lat_out[20]="";
	char       va_lon_out[20]="";
	int i=0;	
	XZAE_LOC_GPS loc_gps;		
	XZSMT_Horodate vl_horo_chaine;
	int	vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
	char vl_date[50]="";
	FILE *file;
	char vl_radio_id[7]="";
	int ret;

	XZST_03EcritureTrace(XZSTC_FONCTION,"apat_envoie_pat_en_cours: IN");

	/* Get Lattitude Longitude à partir de la localisation */

	/* Récuperation de la liste des patrouilleurs en action */
	if (XZAE197_get_pat_radio_date_depart(&loc_gps) != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"apat_envoie_pat_en_cours : pb appel a XZAE197_get_pat_radio_date_depart");
		return;
	}		

	/* ouverture du fichier de destination des données des patrouilleurs en action */
	file = fopen(C_PAT_LIST_FILE_PATH,"w");
	if ( file == NULL ) {
		XZST_03EcritureTrace(XZSTC_WARNING,"apat_envoie_pat_en_cours: Erreur d'ouverture du fichier de destination");
		return;
	}

	/* pour chaque patrouilleur en action */
	for (i = 0 ; i<loc_gps.size ; i++){
		XDY_PR pr_temp =loc_gps.pr[i]/1000;
		
		/* Si le PR n'est pas rond */
		if (loc_gps.pr[i]%1000)
		{
			/* pour sens 2(NORD), il faut renvoyer le PR amont */
			/* Pour l'A57 le PR entre IHM et BASE est deja inverse */		
			if (loc_gps.sens[i]==XDC_SENS_NORD)
			{				
				pr_temp+=1;
			}
		}

		/* conversion de l'horodate au format voulu par le webserver */
		XZSM_11ConversionHorodate(loc_gps.heure_succes[i],&vl_horo_chaine);
		sscanf(vl_horo_chaine, "%2d/%2d/%4d %2d:%2d:%2d",				
				&vl_jour,
				&vl_mois,
				&vl_annee,
				&vl_heure,
				&vl_minute,
				&vl_seconde);
		sprintf(vl_date,"%04d-%02d-%02d %02d:%02d:%02d",
				vl_annee,
				vl_mois,
				vl_jour,				
				vl_heure,
				vl_minute,
				vl_seconde);		
		
		XZST_03EcritureTrace(XZSTC_WARNING,"PAT i=%d :",i);
		XZST_03EcritureTrace(XZSTC_WARNING,"Radio=%s",loc_gps.radio[i]);
		XZST_03EcritureTrace(XZSTC_WARNING,"heure_succes=%s",vl_date);
		XZST_03EcritureTrace(XZSTC_WARNING,"autoroute=%d",loc_gps.autoroute[i]);
		XZST_03EcritureTrace(XZSTC_WARNING,"pr=%d",pr_temp);
		XZST_03EcritureTrace(XZSTC_WARNING,"sens=%d",loc_gps.sens[i]);

		/* récuperation des coordonées à partir de l'autoroute et du pr */
		if (XZAE198_Lire_LOC_GPS (loc_gps.autoroute[i] ,pr_temp,va_lat_out,va_lon_out) != XDC_OK) {
			XZST_03EcritureTrace(XZSTC_WARNING,"apat_envoie_pat_en_cours : pb appel a XZAE198_Lire_LOC_GPS");
			continue;
		}
		/* supréssion des espaces superflu en fin de chaine */
		XZLG300_trim(va_lat_out);
		XZLG300_trim(va_lon_out);

		XZST_03EcritureTrace(XZSTC_WARNING,"Latitude=%s",va_lat_out);
		XZST_03EcritureTrace(XZSTC_WARNING,"Longitude=%s",va_lon_out);
		memccpy(vl_radio_id,&loc_gps.radio[i][5],'\0',5);
		ret=fprintf(file,"%s;%s;%s;%d;%s;0600000000\n",vl_radio_id,va_lat_out,va_lon_out,loc_gps.sens[i],vl_date);
	}
	
	/* close file */
	fclose(file);
		
	/* libération de la mémoire */	
	XZAE197_free_memory(&loc_gps);

	XZST_03EcritureTrace(XZSTC_FONCTION,"apat_envoie_pat_en_cours: OUT");
}
