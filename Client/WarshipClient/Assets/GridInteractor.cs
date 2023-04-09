using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GridInteractor : MonoBehaviour
{
    [SerializeField]
    private Camera theCamera;

    [SerializeField]
    private LayerMask seaLayer;

    // Update is called once per frame
    void Update()
    {
        if(Input.GetMouseButtonDown(0))//left clic
        {
            Ray ray = theCamera.ScreenPointToRay(Input.mousePosition);

            Debug.DrawRay(ray.origin, ray.direction*50f, Color.red, 5);

            float maxDistance = 250f;
            if(Physics.Raycast(ray, out RaycastHit hit, maxDistance, seaLayer))
            {
                GridManager.instance.registerClic(hit.point);
            }
        }
    }
}
