using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TargetingManager : MonoBehaviour
{
    [SerializeField]
    private GameObject flarePrefab;

    private int flarePoolSize = 10;//must be > availableShots to allow for staging flares, letting time for animation to complete
    private FlareAnimator[] flares;
    private int[] flareUseOrder;
    private int[] flareToTarget;
    private int[] targetsOrder;
    private int availableShots = 5;

    private int shotCount = 1;

    private int[] targets;

    private void Start()
    {
        flares = new FlareAnimator[flarePoolSize];
        flareUseOrder = new int[flarePoolSize];
        flareToTarget = new int[flarePoolSize];

        /** setting up pool of flares **/
        for (int i = 0; i < flares.Length; i++)
        {
            GameObject flare = Instantiate(flarePrefab, transform);
            FlareAnimator animator = flare.GetComponent<FlareAnimator>();
            animator.init();

            flares[i] = animator;
        }

        resetStates(availableShots);
    }

    public void resetStates(int newAvailableShots)
    {
        availableShots = newAvailableShots;
        shotCount = 1;

        targets = new int[newAvailableShots];

        targetsOrder = new int[availableShots];

        for (int i = 0; i < newAvailableShots; i++)
        {
            targetsOrder[i] = 0;
            targets[i] = -1;
        }

        for (int i = 0; i < flarePoolSize; i++)
        {
            flareUseOrder[i] = 0;
            flareToTarget[i] = -1;
        }
    }

    public void target(Vector3 worldPos, int gridIndex)
    {
        bool flareAlreadyOnSite = false;
        int onSiteTargetIndex = -1;
        for (int i = 0; i < targets.Length && !flareAlreadyOnSite; i++)
        {
            if(targets[i] == gridIndex)
            {
                flareAlreadyOnSite = true;
                onSiteTargetIndex = i;
            }
        }

        if(flareAlreadyOnSite)
        {
            //Not shooting, but cancelling a target
            int flareToRemove = getFlareFromTargetIndex(onSiteTargetIndex);
            removeFlare(flareToRemove);
            targets[onSiteTargetIndex] = -1;
            targetsOrder[onSiteTargetIndex] = 0;
        }
        else
        {
            int flareIndex = getAvailableFlare();

            //shut down a flare ?
            if(tryGetAvailableTargetSlot(out int targetIndex))
            {
                //No need to shut down
                placeFlare(flareIndex, targetIndex, worldPos);
                targets[targetIndex] = gridIndex;
            }
            else
            {
                //need to shut down
                //Debug.Log("Shutting down");
                int oldestTargetIndex = getOldestTargetIndex();
                int flareToShutIndex = getFlareFromTargetIndex(oldestTargetIndex);

                removeFlare(flareToShutIndex);
                placeFlare(flareIndex, oldestTargetIndex, worldPos);
                targets[oldestTargetIndex] = gridIndex;
            }

        }
        
        shotCount++; //removing a shot count as a shot to allow for flare age sorting (two flares removed will never share the same shotcount)
        
        /*
        string txt = "Targeting: ";
        for (int i = 0; i < targets.Length; i++)
        {
            txt += targets[i] + " ";
        }
        Debug.Log(txt);
        */        
    }

    private void placeFlare(int flareIndex, int targetIndex, Vector3 worldPos)
    {
        //Debug.Log("Placing Flare[" + flareIndex + "] on slot " + targetIndex);
        flareToTarget[flareIndex] = shotCount;
        targetsOrder[targetIndex] = shotCount;

        flares[flareIndex].play();
        flareUseOrder[flareIndex] = shotCount;

        flares[flareIndex].transform.position = worldPos;
    }

    private void removeFlare(int flareIndex)
    {
        flares[flareIndex].stop();
        flareToTarget[flareIndex] = -1;
    }

    private void Update()
    {
        for (int i = 0; i < flares.Length; i++)
        {
            if(flares[i] != null)
            {
                if(flares[i].needsUpdate)
                {
                    flares[i].updateAnimation();
                }
            }
        }
    }

    private int getAvailableFlare()
    {
        int min = flareUseOrder[0];
        int minIndex = 0;
        for (int i = 0; i < flareUseOrder.Length; i++)
        {
            if(flareUseOrder[i] < min)
            {
                min = flareUseOrder[i];
                minIndex = i;
            }
        }
        return minIndex;
    }

    private bool tryGetAvailableTargetSlot(out int index)
    {
        for (int i = 0; i < targetsOrder.Length; i++)
        {
            if(targetsOrder[i] == 0)
            {
                index = i;
                return true;
            }
        }

        index = -1;
        return false;
    }

    private int getOldestTargetIndex()
    {
        int min = targetsOrder[0];
        int minIndex = 0;
        for (int i = 1; i < targetsOrder.Length; i++)
        {
            if(targetsOrder[i] < min)
            {
                min = targetsOrder[i];
                minIndex = i;
            }
        }

        return minIndex;
    }

    private int getFlareFromTargetIndex(int targetIndex)
    {
        int n = targetsOrder[targetIndex];
        for (int i = 0; i < flareToTarget.Length; i++)
        {
            if(flareToTarget[i] == n)
            {
                return i;
            }
        }

        Debug.LogWarning("Could not match flare to target");
        return -1;
    }
}
