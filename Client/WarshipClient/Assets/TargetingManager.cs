using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TargetingManager : MonoBehaviour
{
    [SerializeField]
    private GameObject flarePrefab;

    private FlareAnimator[] flares = new FlareAnimator[5]; //5 is maximum shots
    private int availableShots = 5;
    private int nextIndex = 0;

    public void target(Vector3 worldPos)
    {
        FlareAnimator animator = Instantiate(flarePrefab, worldPos, flarePrefab.transform.rotation, transform).GetComponent<FlareAnimator>();

        if(flares[nextIndex] != null)
        {
            //would like to try pooling here, but delete for now
            flares[nextIndex].stop();
            Destroy(flares[nextIndex].gameObject, 20);
        }

        flares[nextIndex] = animator;

        nextIndex = (nextIndex + 1) % flares.Length;

        animator.init();
        animator.play();
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
}
