using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    public static GameManager instance;
    private void Awake()
    {
        instance = this;
    }

    [SerializeField]
    private ShipScriptableObject[] shipsToPlace;

    private int placingIndex = -1;
    private GameObject placingGhost = null;

    private bool donePlacing = false;


    [SerializeField]
    private Camera theCamera;

    [SerializeField]
    private LayerMask seaLayer;

    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (donePlacing) return;

        if(placingGhost == null)
        {
            //starting or placing a new one
            ++placingIndex;
            if (placingIndex < shipsToPlace.Length)
            {
                placingGhost = Instantiate(shipsToPlace[placingIndex].shipPrefab);
            }
            else
            {
                donePlacing = true;
                return;
            }

        }

        if (tryGetMousePosOnBoard(out Vector3 point))
        {
            placingGhost.transform.position = GridManager.instance.snapWorldToGrid(point);
        }

        if (Input.GetKeyDown(KeyCode.R))
        {
            Debug.Log("Rotate");
            placingGhost.transform.Rotate(new Vector3(0, 90, 0));
        }

        if(Input.GetMouseButtonDown(0))//left clic
        {
            Debug.Log("Place");
            placingGhost = null;
        }
    }

    public bool tryGetMousePosOnBoard(out Vector3 point)
    {
        Ray ray = theCamera.ScreenPointToRay(Input.mousePosition);

        float maxDistance = 250f;
        if (Physics.Raycast(ray, out RaycastHit hit, maxDistance, seaLayer))
        {
            point = hit.point;
            return true;
        }
        else
        {
            point = Vector3.zero;
            return false;
        }
    }
}
