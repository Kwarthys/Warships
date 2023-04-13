using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BuoyAnimator : MonoBehaviour
{
    [SerializeField]
    private float upSpeed = 1f;
    [SerializeField]
    private float upAmp = 1f;
    [SerializeField]
    private float sideSpeed = 1f;
    [SerializeField]
    private float sideAmp = 1f;

    [SerializeField]
    private float blendStart = 0.9f;

    private Quaternion currentTarget;
    private Quaternion nextTarget;
    private Quaternion origin;
    private float slerpT;

    private float upOffset;

    private void Start()
    {
        nextTarget = getRandomQuaternionTarget();
        currentTarget = getRandomQuaternionTarget();
        origin = Quaternion.LookRotation(Vector3.up);

        upOffset = Random.value;
    }

    void Update()
    {
        Vector3 pos = transform.position;
        pos.y = Mathf.Sin(2 * Mathf.PI * upSpeed * (Time.realtimeSinceStartup + upOffset)) * upAmp;
        transform.position = pos;


        //Accelerating motion when swinging away and slowing motion when coming back
        float modifiedT = Mathf.Sin(slerpT * Mathf.PI);

        Quaternion rot = Quaternion.Slerp(origin, currentTarget, modifiedT);

        if(slerpT > blendStart)
        {
            //startBlending in the next target
            float nextT = Mathf.Sin((slerpT- blendStart) * Mathf.PI);
            rot = Quaternion.Slerp(rot, Quaternion.Slerp(origin, nextTarget, nextT), (slerpT - blendStart) / (1- blendStart));
        }

        transform.rotation = rot;

        if(slerpT >= 1f)
        {            
            slerpT = 1- blendStart;
            currentTarget = nextTarget;
            nextTarget = getRandomQuaternionTarget();
        }

        slerpT += Time.deltaTime * sideSpeed;
    }

    private Quaternion getRandomQuaternionTarget()
    {
        Vector3 randomLookDirection = new Vector3(Random.value * 2f - 1, 0, Random.value * 2f - 1); //Not being normalized should not cause any issue, only caring about direction
        return Quaternion.RotateTowards(Quaternion.LookRotation(Vector3.up), Quaternion.LookRotation(randomLookDirection), sideAmp);
    }
}
