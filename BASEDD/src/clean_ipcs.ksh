#!/bin/ksh

# ATTENTION : Dans ce script on part de l'hypothese que la shared memory est affichee en premier :
# Résultat de la commande ipcs :

##################################################################################################

#------ Shared Memory Segments --------
#key        shmid      owner      perms      bytes      nattch     status      
#0x00000000 196609     migrazur   600        393216     2          dest         
#0x000000a6 2621442    migrazur   666        16         3                       
#0x0001e23a 2654211    migrazur   666        2419200    3                       
#0x0002db6d 2686980    migrazur   666        151640     2                       
#0x00009492 2719749    migrazur   666        780        2                       

#------ Semaphore Arrays --------
#key        semid      owner      perms      nsems     
#0x46002b24 33423362   migrazur   666        1         
#0x54002b24 33456131   migrazur   666        1         
#0x5a002b24 33488900   migrazur   666        1         

#------ Message Queues --------
#key        msqid      owner      perms      used-bytes   messages   

##################################################################################################


list_ipcs=`ipcs | cut -d" " -f2`

isSemaphore=false

for i in $list_ipcs
do

  if [ $i = "Shared" ]
    then
      echo "------------------------------------------"
      echo "-- Nettoyage des Shared Memory Segments --"
      echo "------------------------------------------"
      continue
  elif [ $i = "Semaphore" ]
    then
      isSemaphore=true
      echo "------------------------------------"
      echo "-- Nettoyage des Semaphore Arrays --"
      echo "------------------------------------"
      continue
  elif [ $i = "Message" ]
    then
      echo "-------------------"
      echo "-- Fin Nettoyage --"
      echo "-------------------"
      break
  else
    if [ $isSemaphore = true ]
      then
        echo "ipcrm -s $i"
        ipcrm -s $i
    else
        echo "ipcrm -m $i"
        ipcrm -m $i
    fi
  fi
done
ipcs
