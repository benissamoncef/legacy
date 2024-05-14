/*E*/
/* Fichier : $Id: itma_txc.c,v 1.1 2018/08/27 17:09:16 devgfi Exp $        $Revision: 1.1 $        $Date: 2018/08/27 17:09:16 $
----------------------------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
----------------------------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
----------------------------------------------------------------------------------------------------
* MODULE MTXC * FICHIER itma_txc.c
----------------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'exécution de fonctions C par les modules Applix ITMA_Txx.
*
----------------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	24/08/18 : Creation  1.1
------------------------------------------------------------------------------------------------- */

#include	"errno.h"
#include	"time.h"

#include	"elfapi.h"


#define		ELF_SS_FUNCTION		1


static	double	_10_p9 = (double) (1000 * 1000 * 1000);


/*--------------------------------------------------------------------------------------------------
*
*  Gestion des erreurs et messages associés
*
* ------------------------------------------------------------------------------------------------*/

#define		ELF_ERR_NB_ARG		1
#define		ELF_ERR_ARG_INV		2


static char	*tm_err_msgs[] =
{
	NULL,
	"Nombre d'arguments incorrect",
	"Argument invalide"
};

#define		Ax_Elf_Erreur(Code, Objet)		AxError ((Code), tm_err_msgs[Code], (Objet))



/*--------------------------------------------------------------------------------------------------
*
*  Declaration des fonctions supportées
*
* ------------------------------------------------------------------------------------------------*/

elfData		HELLO ();
elfData		ATTENDRE ();


AxCallInfo_t	fTable[] = {
/*	{ "TEST", HELLO, "HELLO", "HELLO()", ELF_SS_FUNCTION }, */
	{ "MIGRAZUR", ATTENDRE, "ATTENDRE", "ATTENDRE()", ELF_SS_FUNCTION },
	{ NULL, NULL, NULL, NULL, 0 }
};


DLL_EXPORT AxCallInfo_t *AxGetCallInfo ()
{
	return (fTable);
}


/*--------------------------------------------------------------------------------------------------
*
*  Definition des fonctions supportées
*
* ------------------------------------------------------------------------------------------------*/


/*
elfData HELLO ()
{
	elfData	retP = NULL;

	retP = AxMakeStrData (-1, "Hello!");
	return (retP);
}
*/




/* -----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Suspend l'exécution de la tâche appelante durant le laps de temps
*  spécifié par la valeur décimale du paramètre (en seconde).
*
---------------------------------------------------------------------------- */

elfData ATTENDRE (argsP)
elfData argsP;
{
	static char	*vl_mon_Nom = "ATTENDRE";
	double		duree_En_Secondes;
	elfData		retP, param1;
	struct timespec	vl_duree, vl_reste;
	int		cr;

	if (AxArraySize (argsP) != 1) {
		Ax_Elf_Erreur (ELF_ERR_NB_ARG, vl_mon_Nom);
	}

	param1 = AxArrayElement (argsP, 0);
	if (! AxIsNumber (param1)) {
		Ax_Elf_Erreur (ELF_ERR_ARG_INV, vl_mon_Nom);
	}

	duree_En_Secondes = AxFloatFromDataPtr (param1);
	if (duree_En_Secondes < 0.0) {
		Ax_Elf_Erreur (ELF_ERR_ARG_INV, vl_mon_Nom);
	}

	vl_reste.tv_sec  = (time_t) (int) duree_En_Secondes;
	vl_reste.tv_nsec = (long) ((duree_En_Secondes - (double) vl_reste.tv_sec) * _10_p9);

	do {
		vl_duree = vl_reste;
		cr = nanosleep (&vl_duree, &vl_reste);
	} while ((cr == (-1))  &&  (errno == EINTR));

	return ((elfData) NULL);
}

