using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GridInteractor : MonoBehaviour
{
    // Update is called once per frame
    void Update()
    {
        if(Input.GetMouseButtonDown(0))//left clic
        {
            if(GameManager.instance.tryGetMousePosOnBoard(out Vector3 point))
            {
                //GridManager.instance.registerClic(point);
            }
        }
    }
}
