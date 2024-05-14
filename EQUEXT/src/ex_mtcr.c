/*E*/
/* Fichier : $Id: ex_mtcr.c,v 1.13 2019/01/29 11:10:58 pc2dpdy Exp $        Release : $Revision: 1.13 $        Date : $Date: 2019/01/29 11:10:58 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mtcr.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fonctions d'encapsulation message LCR, calcul CRC communes
* au Sous-Systeme EQUEXT
* 
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	24 Oct 1994	: Creation
* Flaidar.A	version 1.2	01 Dec 1994	: Correction erreur verif. entete requette TEDI dans
*						: ex_verif_TEDI_CRC().
* Flaidar.A	version 1.3	01 Dec 1994	: Correction de la correction.
* Mismer.D	version 1.4	06 Jan 1995	:
* Mismer.D	version 1.5	25 Jan 1995	:
* Mismer.D	version 1.6	08 Mar 1995	:
* Fontaine.C	version 1.7	23 Mai 1995	: Reprise entete, historique
* PNI		version 1.9 	01/09/08	: Ajout trace CRC
* PNI		version 1.10	26/06/2009	: Verif TEDI : on ne regarde que les 2 premiers car de l'@ rgs
*						  pour eviter la lettre du site durant les modif secto DEM887
* JPL		24/02/11 : Migration architecture HP ia64 (DEM 975) : retour C.R. OK pour fonction 'ex_verif_TEDI_CRC'  1.12
* LCL		29/03/18 : Ajout calcul BCC pour DIASER CTRL_FEUX 1.13 DEM1284
------------------------------------------------------*/

/* fichiers inclus */

#include "ex_mtcr.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: ex_mtcr.c,v 1.13 2019/01/29 11:10:58 pc2dpdy Exp $ : ex_mtcr" ;

/* declaration de fonctions internes */

int ex_verif_TEDI_CRC( XZEXT_Msg_Socket *, char *, char * );
char EX_calcul_CRC( char * );
int EX_encapsuler_LCR( XZEXT_Comp_Trame_TEDI *, XZEXT_Msg_TEDI * );
int EX_encapsuler_LCR_IP( XZEXT_Comp_Trame_TEDI *, XZEXT_Msg_TEDI * );
char * EX_affiche_TEDI( XZEXT_Msg_TEDI * );
void EX_saisie_cmd_LCR( char *, int  );
char EX_calcul_CRC_DIASER( char * );


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Calcul valeur CRC de la chaine des cars. recue en argument
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*	char EX_calcul_CRC( char *pa_MsgTEDI )
*
*
* ARGUMENTS EN ENTREE :
*   pa_MsgTEDI	: pointeur sur chaine de caracteres contenant le message TEDI sans le CRC.
*
* ARGUMENTS EN SORTIE :
*   neant
*
* CODE RETOUR : 
*   vl_ValCRC	: la valeur du caract�re CRC sur 7 bits significatifs.
* 
* CONDITION D'UTILISATION
*   la chaine passe en argument doit se terminer avec '\0'.
*
* FONCTION 
*   calcule la somme des caracteres de la chaine recu en argument sur 8 bits.
*
------------------------------------------------------*/
char EX_calcul_CRC( char *pa_MsgTEDI )
{
    unsigned char 	vl_ValCRC = XZEXC_NULC;		/* valeur CRC calcul� - init. a 0x00 */
    char		*pl_Tmp;			/* pointeur qui "glisse" sue la chaine */
 
    /*A pl_Tmp "glisse" de pa_MsgTEDI au premier caractere '\0' */
    for( pl_Tmp = pa_MsgTEDI; *pl_Tmp; pl_Tmp++ ) {
    	/*B a chaque incrementation pl_Tmp on rajoute sa valeur a vl_ValCRC */
    	vl_ValCRC += *pl_Tmp;
/*	XZST_03EcritureTrace( XZSTC_WARNING," CRC %d-%d",vl_ValCRC,*pl_Tmp);    	
*/    }	
/*	XZST_03EcritureTrace( XZSTC_WARNING," CRC final %d", (char)(vl_ValCRC &= XZEXC_CRC_MASK));
*/
	return( (char)(vl_ValCRC &= XZEXC_CRC_MASK) );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Calcul valeur CRC de la chaine des cars. recue en argument pour trames DIASER
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*	char EX_calcul_CRC_DIASER( char *pa_MsgTEDI )
*
*
* ARGUMENTS EN ENTREE :
*   pa_MsgTEDI	: pointeur sur chaine de caracteres contenant le message DIASER sans le CRC.
*
* ARGUMENTS EN SORTIE :
*   neant
*
* CODE RETOUR : 
*   vl_ValCRC	: la valeur du caractere CRC : ou exclusif de tous les caracteres en commencant au deuxieme (specificite DIASER)
* 
* CONDITION D'UTILISATION
*   la chaine passe en argument doit se terminer avec '\0'.
*
* FONCTION 
*   calcule la somme des caracteres de la chaine recu en argument en commencant au deuxieme sur 8 bits.
*
------------------------------------------------------*/
char EX_calcul_CRC_DIASER( char *pa_MsgTEDI )
{
    unsigned char 	vl_ValCRC = XZEXC_NULC;		/* valeur CRC calcul� - init. a 0x00 */
    char		*pl_Tmp;			/* pointeur qui "glisse" sue la chaine */
 
    /*A pl_Tmp "glisse" de pa_MsgTEDI au premier caractere '\0' */
    for( pl_Tmp = pa_MsgTEDI+1; *pl_Tmp; pl_Tmp++ ) {
    	/*B a chaque incrementation pl_Tmp on rajoute sa valeur a vl_ValCRC */
    	vl_ValCRC ^= *pl_Tmp;
	XZST_03EcritureTrace( XZSTC_WARNING," CRC DIASER %d-%d",vl_ValCRC,*pl_Tmp);    	
    }	
    return( (char)(vl_ValCRC) );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Encapsulation d'une trame LCR en trame TEDI/LCR.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*	int EX_encapsuler_LCR( XZEXT_Comp_Trame_TEDI *pa_BriquesTEDI, XZEXT_Msg_TEDI *pa_MsgTEDI )
*
*
* ARGUMENTS EN ENTREE :
*   pa_BriquesTEDI : pointeur sur struct. donnees contenant les composants de le future trame TEDI   
*
* ARGUMENTS EN SORTIE :
*   pa_MsgTEDI	: pointeur sur structure message TEDI/LCR ( trame et sa longueur )
*
* CODE RETOUR : 
*   XDC_OK 	: si pas de probleme d'encapsulation.
*   XDC_NOK	: si requette avec no. bloc != 0 ou type message != requette, info ou acquit.
* 
* CONDITION D'UTILISATION
*   pa_BriquesTEDI->trame_LCR pointe sur une chaine de max. 247 cars. (256-247=9 cars. min. nec. pour l'encapsulation).
*   les chaines pointees par pa_BriquesTEDI->trame_LCR et pa_BriquesTEDI->adr_RGSB doivent de terminer avec '\0'.
*   si encaps. mess. TEDI acquitt. => acquitt. ( [ACK]b ou [NAK]b ) stoke ds. pa_BriquesTEDI->adr_RGSB
*
* FONCTION 
*   encapsule selon protocole TEDI/LCR pa_BriquesTEDI en pa_MsgTEDI->mesgTEDI en utilisant les cars. speciaux de
*   controle du protocole et la fonction EX_calcule_CRC().
*
------------------------------------------------------*/
int EX_encapsuler_LCR( 	XZEXT_Comp_Trame_TEDI *pa_BriquesTEDI, 
			XZEXT_Msg_TEDI *pa_MsgTEDI )
{
    char 	*pl_Tmp;	/* pointeur sur char pour "glisser" sur une chaine de cars. */
    short 	i;		/* compteur(s) */

    /*A initialisation de la trame TEDI */
    pa_MsgTEDI->trame_TEDI[0] = '\0';
    pa_MsgTEDI->len = 0;    

    /*A Creation prefixe ds. la trame TEDI */
    for( pl_Tmp = pa_MsgTEDI->trame_TEDI, i = 0; i < pa_BriquesTEDI->long_pref; i++, pl_Tmp++ )
    	*pl_Tmp = XZEXC_CAR_PREF;
    
    /*A Mise a jour longueur trame TEDI */
    
    pa_MsgTEDI->len += pa_BriquesTEDI->long_pref;
    
    /*A Cas type du message TEDI : requette, info ou acquittement */
    switch( pa_BriquesTEDI->type_msg )
    {
    case XZEXC_MSG_REQ :	/* FALL THROUGH !!! */
    	/*A cas : message requette = [ENQ]rgs0 CMDE_LCR [ETX][BCC] */
    	/*! attention : traitement commun cas message requette et cas message info !!! */
    case XZEXC_MSG_INFO :
    case XZEXC_BLC_INFO :
    	/*A cas : message ou bloc info = [STX]rgsb INFO_LCR [ETX]ou[ETB][BCC] */
    	/*B formatage trame TEDI sans CRC */
    	sprintf( 	pl_Tmp, 
   			"%c%s%c%s%c",
    			pa_BriquesTEDI->type_msg == XZEXC_MSG_REQ ? XZEXC_CAR_ENQ : XZEXC_CAR_STX,
    			pa_BriquesTEDI->adr_RGSB,
    			XZEXC_CAR_SEP,
    			pa_BriquesTEDI->trame_LCR,
    		/*	XZEXC_CAR_SEP,*/
    			pa_BriquesTEDI->type_msg == XZEXC_BLC_INFO ? XZEXC_CAR_ETB : XZEXC_CAR_ETX
    		);
    	
    	/*B calcul et concatenation CRC, mise a jour longueur trame TEDI */
    	pa_MsgTEDI->len += strlen( pl_Tmp );
    	pa_MsgTEDI->trame_TEDI[pa_MsgTEDI->len] = EX_calcul_CRC( pl_Tmp );
    	pa_MsgTEDI->len++;
    	
    	/*A si message type requette et no. bloc != '0' => trace et retour XDC_NOK */
    	if( pa_BriquesTEDI->type_msg == XZEXC_MSG_REQ && pa_BriquesTEDI->adr_RGSB[3] != '0' )
    	{
	    /*A Ecriture trace "  Message TEDI type requette avec no. bloc x " */
	    XZST_03EcritureTrace( XZSTC_WARNING, " Message TEDI type requette avec no. bloc %c.",
	    			  pa_BriquesTEDI->adr_RGSB[3] ); 
            /*A Sortie de la fonction */
            return ( XDC_NOK );
    	}
    	break;

    case XZEXC_MSG_ACQUIT :
    	/*A cas : message acquittement ( pos. ou neg. ) = [ACK]b ou [NAK]b */
    	/*B formatage trame TEDI, acquittement contenu ds. le champ adr_RGSB */
    	sprintf(	pl_Tmp,
    			"%s",
    			pa_BriquesTEDI->adr_RGSB
    		);
    	/*B mise a jour longueur trame TEDI */
    	pa_MsgTEDI->len += strlen( pl_Tmp );
    	break;
    	
    default :
    	/*A cas par defaut : type message inconnu => trace et retour XDC_NOK */
    	/*A Ecriture trace type message inconnu */
	XZST_03EcritureTrace( 	XZSTC_WARNING, " Message de type %d inconnu . ", 
				pa_BriquesTEDI->type_msg); 
        /*A Sortie de la fonction */
        return ( XDC_NOK );

    }
    
    /*A concatenation suffixe a la trame TEDI, mise a jour longueur trame TEDI */
    for( 	pl_Tmp = &(pa_MsgTEDI->trame_TEDI[pa_MsgTEDI->len]), i = 0; 
    		i < pa_BriquesTEDI->long_suff; 
    		i++, pl_Tmp++ 
    	)
    	*pl_Tmp = XZEXC_CAR_SUFF;
    
    pa_MsgTEDI->len += pa_BriquesTEDI->long_suff;
    pa_MsgTEDI->trame_TEDI[pa_MsgTEDI->len] = XZEXC_NULC;
    
    return( XDC_OK );
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Mise en format lisible d'un trame TEDI/LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*	char * EX_affiche_TEDI( XZEXT_Msg_TEDI *pa_MsgTEDI )
*
*
* ARGUMENTS EN ENTREE :
*   pa_MsgTEDI	: pointeur sur struct. donnees contenant la trame TEDI   
*
* ARGUMENTS EN SORTIE :
*   NEANT
*
* CODE RETOUR : 
*   char * 	: pointeur sur la chaine contenant la trame TEDI en format lisible
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   met en format lisible une trame TEDI/LCR
*   
*
------------------------------------------------------*/
char * EX_affiche_TEDI( XZEXT_Msg_TEDI *pa_MsgTEDI )
{
    char 	*pl_FormatLisible;	/* pointeur sur trame en format lisible */
    int		i, j, k;		/* compteur(s) */
    int		len;
    
    
    /*A allouer m�moire pour la trame en format lisible */
    pl_FormatLisible = malloc( sizeof(char)*512 );
    
    /*A comptage cars. prefixe et suffixe*/
    for( i = 0; i < pa_MsgTEDI->len && 
    		pa_MsgTEDI->trame_TEDI[i] != XZEXC_CAR_ACK &&
    		pa_MsgTEDI->trame_TEDI[i] != XZEXC_CAR_NAK &&
    		pa_MsgTEDI->trame_TEDI[i] != XZEXC_CAR_STX &&
    		pa_MsgTEDI->trame_TEDI[i] != XZEXC_CAR_ENQ; i++ );
    		
    /*A SI chaine parcourue sans trouver les cars. de controle */
    if( i == pa_MsgTEDI->len )
    {
    	/*A ALORS RETOUR "MESSAGE NON TEDI/LCR, ERR. DDM !" */
    	strcpy( pl_FormatLisible, "MESSAGE NON TEDI/LCR , ERR. DDM !\n\n" );
    	return( pl_FormatLisible );
    }

    /*A Mise en format lisible de la trame */

    sprintf( pl_FormatLisible, "%s%d", "\nNbre. cars. pref. : ", i );
    strcat( pl_FormatLisible, "\nTrame : " );

    /*A cas 1er car. de la trame TEDI/LCR */   
    switch( pa_MsgTEDI->trame_TEDI[i] )
    {
    /*A cas 1er car = [ENQ] */
    case XZEXC_CAR_ENQ :	/* FALL THROUGH !!!*/
    /*A cas 1er car = [STX] */
    case XZEXC_CAR_STX :
    	/*B detection fin message */
    	for( j = i; 	j < pa_MsgTEDI->len &&
    			pa_MsgTEDI->trame_TEDI[j] != XZEXC_CAR_ETX &&
    			pa_MsgTEDI->trame_TEDI[j] != XZEXC_CAR_ETB; j++ );
    	/*B SI chaine parcourue sans trouver le FDM */
    	if( j == pa_MsgTEDI->len )
    	{
    	    /*B ALORS RETOUR "MESSAGE NON TEDI/LCR , ERR. FDM !" */
    	    strcpy( pl_FormatLisible, "MESSAGE NON TEDI/LCR , ERR. FDM !\n\n" );
    	    return( pl_FormatLisible );
    	}

	/*B mise en format lisible */
	
    	strcat( pl_FormatLisible, pa_MsgTEDI->trame_TEDI[i] == XZEXC_CAR_ENQ ? "[ENQ]" : "[STX]" );
    	strncat( pl_FormatLisible, &(pa_MsgTEDI->trame_TEDI[i+1]), (size_t) j - i );
    	strcat( pl_FormatLisible, 
    		pa_MsgTEDI->trame_TEDI[j] == XZEXC_CAR_ETX ? "[ETX][BCC]" : "[ETB][BCC]" 
    		);
    	sprintf( &pl_FormatLisible[strlen(pl_FormatLisible)], "\n\t[BCC] = %.2X", 
    		 pa_MsgTEDI->trame_TEDI[j+1]
  		);
    	break;

    /*A cas 1er car = [ACK] */
    case XZEXC_CAR_ACK :	/* FALL THROUGH !!!*/
    /*A cas 1er car = [NAK] */
    case XZEXC_CAR_NAK :
    	strcat( pl_FormatLisible, pa_MsgTEDI->trame_TEDI[i] == XZEXC_CAR_ACK ? "[ACK]" : "[NAK]" );
    	strncat( pl_FormatLisible, &(pa_MsgTEDI->trame_TEDI[i+1]), 1 );
	j = i;
    	break;
    
    /*A cas par defaut : normallement jamais atteint */
    default :
    	break;	 
    }

    sprintf( &pl_FormatLisible[strlen(pl_FormatLisible)], "%s%d\n\n", 
    		"\nNbre. cars. suff. : ", pa_MsgTEDI->len - j - 2 );
    return(  pl_FormatLisible );
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Saisie au clavier d'une chaine de cars. contenant des espaces
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*	void EX_saisie_cmd_LCR( char *pa_CmdLCR, int va_MaxLong )
*
*
* ARGUMENTS EN ENTREE :
*   va_MaxLong	: longueur max. de la chaine a saisir
*
* ARGUMENTS EN SORTIE :
*   pa_CmdLCR	: pointeur sur la zone mem. pour stoker la chaine saisie.
*
* CODE RETOUR : 
*   NEANT
*
* CONDITION D'UTILISATION
*   pa_CmdLCR	: pointe sur un buffer de min. va_MaxLong + 1 cars.
*
* FONCTION 
*   Saisie d'une chaine de max va_MaxLong cars. au clavier. Fin de la chaine = touche Entree.
*   
*
------------------------------------------------------*/
void EX_saisie_cmd_LCR( char *pa_CmdLCR, int va_MaxLong )
{
    char	*pl_Tmp;	/* pointeur qui "glisse" a partir de pa_CmdLCR */
    int		i;		/* compteur(s) */
    
    
    /*A boucle pour la saisie successive des cars. au clavier 
     *	tant que '\n' pas encore saisi ou i < va_MaxLong
     */
    
    i = 0;
    pl_Tmp = pa_CmdLCR;
    fflush( stdin );
    while( (*pl_Tmp = getchar()) != '\n' && i < va_MaxLong )
    {
        pl_Tmp++;
        i++;
    }
    *pl_Tmp = XZEXC_NULC;
    fflush( stdin );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Verification du CRC et de la structure TEDI de la chaine passee en argument.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*	int ex_verif_TEDI_CRC( XZEXT_Msg_Socket *pa_MsgSock, char *pa_DebutMsg, char *pa_NoBloc)
*
* ARGUMENTS EN ENTREE :
*
* pa_MsgSock	: pointeur sur message recu sur socket ligne serie.
* pa_DebutMsg	: debut de message ([ENQ]rgs ou [STX]rgs) a verifier.
* pa_NoBloc	: no. bloc attendu pour les acquitt.
*
* ARGUMENTS EN SORTIE :
* 
* pa_NoBloc	: indique le no. du bloc du message recu.
*
* CODE RETOUR : 
*
* XZEXC_MSG_OK ou XZEXC_BLC_OK		 : si structure TEDI correcte (cars. speciaux et CRC ).
* XZEXC_ERR_CRC_BLC ou XZEXC_ERR_CRC_MSG : si erreur CRC message ou bloc interm.
* XZEXC_ACK_OK ou XZEXC_NAK_OK		 : si acquitt. pos. ou neg. correcte.
* XZEXC_TEDI_NOK : si message receptionne sans structure TEDI ou no. bloc incorrecte pour les acquitt.
*
* CONDITION D'UTILISATION
* pa_MsgSock->msg de longueur pa_MsgSock->len.
* si le code retour est un _OK, pa_MsgSock->msg contient le msg. LCR (sans pref., suff., DDM, FDM).
*
* FONCTION 
*
* - verification du CRC.
* - verification presence des caracteres speciaux et du debut de message correcte.
* - le debut du message et donne par pa_DebutMsg - [STX]rgs pour TELMi, [ENQ]rgs pour TELEi.
*
------------------------------------------------------*/
int ex_verif_TEDI_CRC( XZEXT_Msg_Socket *pa_MsgSock, char *pa_DebutMsg, char *pa_NoBloc)
{
    static char *version = "$Id: ex_mtcr.c,v 1.13 2019/01/29 11:10:58 pc2dpdy Exp $ : ex_verif_TEDI_CRC" ;
    
    char	vl_CRC;		/* stokage valeur BCC trouvee ds. la chaine */
    int		i, j;		/* compteur(s) */
    
   XZST_03EcritureTrace( XZSTC_WARNING, "pa_MsgSock %s",pa_MsgSock); 
   XZST_03EcritureTrace( XZSTC_WARNING, "pa_DebutMsg %s",pa_DebutMsg); 
  
    /*A recherche 1er car. pa_DebutMsg a partir du debut du message */
    for( i = 0; i < pa_MsgSock->len && pa_MsgSock->msg[i] != pa_DebutMsg[0]; i++ )
    {
    	/*B detection acquittement */
    	/*B CAS valeur du caractere courant */
    	switch( pa_MsgSock->msg[i] )
    	{
    	/*B cas XZEXC_CAR_ACK ou XZEXC_CAR_NAK -> meme traitement */
    	case XZEXC_CAR_ACK :
    	case XZEXC_CAR_NAK :
	    /*B SI le no. du bloc acquitte est le bon (passe ds. l'argument pa_NoBloc) */
	    if( pa_MsgSock->msg[i+1] == *pa_NoBloc )
	    {
	    	/*B ALORS elimination prefixe et suffixe */
		pa_MsgSock->msg[0] = pa_MsgSock->msg[i];
		pa_MsgSock->msg[XZEXC_LONG_ACQUIT_TEDI-1] = pa_MsgSock->msg[i+1];
	    	pa_MsgSock->msg[pa_MsgSock->len=XZEXC_LONG_ACQUIT_TEDI] = XZEXC_NULC;
	    	/*B RETOUR XZEXC_ACK_OK ou XZEXC_NAK_OK */
	    	return( pa_MsgSock->msg[0] == XZEXC_CAR_ACK ? XZEXC_ACK_OK : XZEXC_NAK_OK );
    	    }
    	    /*B SINON RETOUR XZEXC_TEDI_NOK */
    	    XZST_03EcritureTrace( XZSTC_WARNING, "Cas 1 de XZEXC_TEDI_NOK");
		return( XZEXC_TEDI_NOK );
    	    break;
    	    
    	/*B cas par defaut -> continue la recherche */
    	default :
    	    break;
    	}
    }

    /*A SI chaine entierement parcourue sans trouver le debut RETOUR XZEXC_TEDI_NOK */
    if( i == pa_MsgSock->len )
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "Cas 2 de XZEXC_TEDI_NOK");
	return( XZEXC_TEDI_NOK );
    }

    /*A SI le message n'a pas le bon DDM RETOUR XZEXC_TEDI_NOK */
/*    if( strncmp( pa_DebutMsg, &pa_MsgSock->msg[i], XZEXC_LONG_DDM_TEDI-1 ) != 0 )*/ /**DEM887; on de regarde que les 2 premier car de l'@ rgs pour eviter la lettre du site durant les modif secto */
    if( strncmp( pa_DebutMsg, &pa_MsgSock->msg[i], XZEXC_LONG_DDM_TEDI-3 ) != 0 )
    {
    	XZST_03EcritureTrace( XZSTC_WARNING, "Cas 3 de XZEXC_TEDI_NOK:pa_DebutMsg=%s; &pa_MsgSock->msg[i]=%s;i=%d;long=%d",pa_DebutMsg, &pa_MsgSock->msg[i],i,XZEXC_LONG_DDM_TEDI-1);
	return( XZEXC_TEDI_NOK );
    }
    
    /*A init. val vl_CRC a XZEXC_NULC */
    vl_CRC = XZEXC_NULC;
    
    /*A TANT QUE car. de fin de message non trouve et dernier car. du message pas atteint */
    for( j = i; j <= pa_MsgSock->len-1; j++ )
    {
    	/*B CAS caractere courant */
    	switch( pa_MsgSock->msg[j] )
    	{
    	/*B cas XZEXC_CAR_ETB */
    	case XZEXC_CAR_ETB :
    	    /*B SI msg. selection ENQ termine par ETB ALORS erreur TEDI */
    	    if( pa_DebutMsg[0] == XZEXC_CAR_ENQ )
    	    {
    	    	XZST_03EcritureTrace( XZSTC_WARNING, "Cas 4 de XZEXC_TEDI_NOK");
		return( XZEXC_TEDI_NOK );
    	    }
    	    /*! traitement commun ETB et ETX */
    	
    	/*B cas XZEXC_CAR_ETX */
    	case XZEXC_CAR_ETX :
    	    /*B SI requette avec no. bloc diff. de XZEXC_CAR_NO_BLC_REQ = '0' */
    	    if( pa_DebutMsg[0] == XZEXC_CAR_ENQ 
    	    	&& pa_MsgSock->msg[i+XZEXC_LONG_DDM_TEDI-1] != XZEXC_CAR_NO_BLC_REQ )
    	    {
    	    	/*B ALORS RETOUR XZEXC_TEDI_NOK */
    	    	XZST_03EcritureTrace( XZSTC_WARNING, "Cas 5 de XZEXC_TEDI_NOK");
		return( XZEXC_TEDI_NOK );
    	    }
    	    /*B ajout dernier val. car. au CRC */
    	    vl_CRC += pa_MsgSock->msg[j];
    	    
    	    /*B sauvegarde no. bloc */
    	    *pa_NoBloc = pa_MsgSock->msg[i+XZEXC_LONG_DDM_TEDI-1];
    	    
    	    XZST_03EcritureTrace( XZSTC_INFO, "CRC verif pa_MsgSock->msg=%s pa_MsgSock->len=%d",pa_MsgSock->msg,pa_MsgSock->len);
	    XZST_03EcritureTrace( XZSTC_INFO, "CRC verif (vl_CRC=%d) = (pa_MsgSock->msg[j+1]=%d) ?",(vl_CRC & XZEXC_CRC_MASK),pa_MsgSock->msg[j+1]);
    	    /*B SI CRC incorrecte RETOUR XZEXC_ERR_CRC_MSG ou XZEXC_ERR_CRC_BLC */
    	    if( (vl_CRC & XZEXC_CRC_MASK) != pa_MsgSock->msg[j+1] )
    	    {
    	    	XZST_03EcritureTrace( XZSTC_WARNING, "CRC incorrecte pa_MsgSock->msg=%s pa_MsgSock->len=%d",pa_MsgSock->msg,pa_MsgSock->len);
		XZST_03EcritureTrace( XZSTC_WARNING, "CRC incorrecte (vl_CRC=%d) != (pa_MsgSock->msg[j+1]=%d,%d) ; pa_MsgSock->msg[j]=%d attendu  %d",(vl_CRC & XZEXC_CRC_MASK),pa_MsgSock->msg[j+1],(pa_MsgSock->msg[j+1] & XZEXC_CRC_MASK),pa_MsgSock->msg[j],XZEXC_CAR_ETX);
		return( pa_MsgSock->msg[j] == XZEXC_CAR_ETX ? XZEXC_ERR_CRC_MSG : XZEXC_ERR_CRC_BLC);
    	    }
    	    
    	    /*B elimination prefixe, suffixe, DDM et FDM */
    	    pa_MsgSock->len = j - i + 2 - XZEXC_LONG_DDM_TEDI - XZEXC_LONG_FDM_TEDI;
    	    strncpy( pa_MsgSock->msg, &pa_MsgSock->msg[i+XZEXC_LONG_DDM_TEDI], pa_MsgSock->len );
    	    pa_MsgSock->msg[pa_MsgSock->len] = XZEXC_NULC;

    	    /*B RETOUR XZEXC_MSG_OK ou XZEXC_BLC_OK */
    	    return( pa_MsgSock->msg[j] == XZEXC_CAR_ETX ? XZEXC_MSG_OK : XZEXC_BLC_OK );
    	    break;
    	    
    	/*B cas par defaut : somme CRC */
    	default :
    	    vl_CRC += pa_MsgSock->msg[j];
    	    break;
    	}
    }

    /* JPL 24/02/11 return (???) - pour rendre le code deterministe ... */
    return (XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Encapsulation d'une trame LCR en trame TEDI/LCR. IP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*	int EX_encapsuler_LCR_IP( XZEXT_Comp_Trame_TEDI *pa_BriquesTEDI, XZEXT_Msg_TEDI *pa_MsgTEDI )
*
*
* ARGUMENTS EN ENTREE :
*   pa_BriquesTEDI : pointeur sur struct. donnees contenant les composants de le future trame TEDI   
*
* ARGUMENTS EN SORTIE :
*   pa_MsgTEDI	: pointeur sur structure message TEDI/LCR ( trame et sa longueur )
*
* CODE RETOUR : 
*   XDC_OK 	: si pas de probleme d'encapsulation.
*   XDC_NOK	: si requette avec no. bloc != 0 ou type message != requette, info ou acquit.
* 
* CONDITION D'UTILISATION
*   pa_BriquesTEDI->trame_LCR pointe sur une chaine de max. 247 cars. (256-247=9 cars. min. nec. pour l'encapsulation).
*   les chaines pointees par pa_BriquesTEDI->trame_LCR et pa_BriquesTEDI->adr_RGSB doivent de terminer avec '\0'.
*   si encaps. mess. TEDI acquitt. => acquitt. ( [ACK]b ou [NAK]b ) stoke ds. pa_BriquesTEDI->adr_RGSB
*
* FONCTION 
*   encapsule selon protocole TEDI/LCR pa_BriquesTEDI en pa_MsgTEDI->mesgTEDI en utilisant les cars. speciaux de
*   controle du protocole et la fonction EX_calcule_CRC().
*
------------------------------------------------------*/
int EX_encapsuler_LCR_IP( 	XZEXT_Comp_Trame_TEDI *pa_BriquesTEDI, 
			XZEXT_Msg_TEDI *pa_MsgTEDI )
{
    char 	*pl_Tmp;	/* pointeur sur char pour "glisser" sur une chaine de cars. */
    short 	i;		/* compteur(s) */

    /*A initialisation de la trame TEDI */
    pa_MsgTEDI->trame_TEDI[0] = '\0';
    pa_MsgTEDI->len = 0;    

    /*A Creation prefixe ds. la trame TEDI */
    for( pl_Tmp = pa_MsgTEDI->trame_TEDI, i = 0; i < pa_BriquesTEDI->long_pref; i++, pl_Tmp++ )
    	*pl_Tmp = XZEXC_CAR_PREF;
    
    /*A Mise a jour longueur trame TEDI */
    
    pa_MsgTEDI->len += pa_BriquesTEDI->long_pref;
    
    /*A Cas type du message TEDI : requette, info ou acquittement */
    switch( pa_BriquesTEDI->type_msg )
    {
    case XZEXC_MSG_REQ :	/* FALL THROUGH !!! */
    	/*A cas : message requette = [ENQ]rgs0 CMDE_LCR [ETX][BCC] */
    	/*! attention : traitement commun cas message requette et cas message info !!! */
    case XZEXC_MSG_INFO :
    case XZEXC_BLC_INFO :
    	/*A cas : message ou bloc info = [STX]rgsb INFO_LCR [ETX]ou[ETB][BCC] */
    	/*B formatage trame TEDI sans CRC */
    	sprintf( 	pl_Tmp, 
   			/*"%c%s%c%s%c",*/
   			"%c%s%s%c",
    			pa_BriquesTEDI->type_msg == XZEXC_MSG_REQ ? XZEXC_CAR_ENQ : XZEXC_CAR_STX,
    			pa_BriquesTEDI->adr_RGSB,
    			/*XZEXC_CAR_SEP,*/
    			pa_BriquesTEDI->trame_LCR,
    		/*	XZEXC_CAR_SEP,*/
    			pa_BriquesTEDI->type_msg == XZEXC_BLC_INFO ? XZEXC_CAR_ETB : XZEXC_CAR_ETX
    		);
    	
    	/*B calcul et concatenation CRC, mise a jour longueur trame TEDI */
    	pa_MsgTEDI->len += strlen( pl_Tmp );
    	pa_MsgTEDI->trame_TEDI[pa_MsgTEDI->len] = EX_calcul_CRC( pl_Tmp );
    	pa_MsgTEDI->len++;
    	
    	/*A si message type requette et no. bloc != '0' => trace et retour XDC_NOK */
    	if( pa_BriquesTEDI->type_msg == XZEXC_MSG_REQ && pa_BriquesTEDI->adr_RGSB[3] != '0' )
    	{
	    /*A Ecriture trace "  Message TEDI type requette avec no. bloc x " */
	    XZST_03EcritureTrace( XZSTC_WARNING, " Message TEDI type requette avec no. bloc %c.",
	    			  pa_BriquesTEDI->adr_RGSB[3] ); 
            /*A Sortie de la fonction */
            return ( XDC_NOK );
    	}
    	break;

    case XZEXC_MSG_ACQUIT :
    	/*A cas : message acquittement ( pos. ou neg. ) = [ACK]b ou [NAK]b */
    	/*B formatage trame TEDI, acquittement contenu ds. le champ adr_RGSB */
    	sprintf(	pl_Tmp,
    			"%s",
    			pa_BriquesTEDI->adr_RGSB
    		);
    	/*B mise a jour longueur trame TEDI */
    	pa_MsgTEDI->len += strlen( pl_Tmp );
    	break;
    	
    default :
    	/*A cas par defaut : type message inconnu => trace et retour XDC_NOK */
    	/*A Ecriture trace type message inconnu */
	XZST_03EcritureTrace( 	XZSTC_WARNING, " Message de type %d inconnu . ", 
				pa_BriquesTEDI->type_msg); 
        /*A Sortie de la fonction */
        return ( XDC_NOK );

    }
    
    /*A concatenation suffixe a la trame TEDI, mise a jour longueur trame TEDI */
    for( 	pl_Tmp = &(pa_MsgTEDI->trame_TEDI[pa_MsgTEDI->len]), i = 0; 
    		i < pa_BriquesTEDI->long_suff; 
    		i++, pl_Tmp++ 
    	)
    	*pl_Tmp = XZEXC_CAR_SUFF;
    
    pa_MsgTEDI->len += pa_BriquesTEDI->long_suff;
    pa_MsgTEDI->trame_TEDI[pa_MsgTEDI->len] = XZEXC_NULC;
    
    return( XDC_OK );
}

