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

    public GridManager playerBoard;
    public GridManager opponentBoard;

    [SerializeField]
    private ShipScriptableObject[] shipsToPlace;
    [SerializeField]
    private Ship[] ships;

    private int placingIndex = -1;
    private GameObject placingGhost = null;
    private int ghostLength;
    private bool donePlacing = false;


    [SerializeField]
    private Camera theCamera;

    [SerializeField]
    private LayerMask seaLayer;

    void Start()
    {
        ships = new Ship[shipsToPlace.Length];
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
                ghostLength = shipsToPlace[placingIndex].length;

                ships[placingIndex] = new Ship(shipsToPlace[placingIndex], placingIndex);
            }
            else
            {
                donePlacing = true;
                return;
            }

        }

        bool inGrid = false;
        Vector3 point;
        if (tryGetMousePosOnBoard(out point))
        {
            if(playerBoard.snapShipToGrid(point, out Vector3 pos, ships[placingIndex].length, ships[placingIndex].orientation))
            {
                placingGhost.transform.position = pos;
                inGrid = true;
            }
        }

        if (Input.GetKeyDown(KeyCode.R))
        {
            ships[placingIndex].rotate();
            placingGhost.transform.Rotate(new Vector3(0, 90, 0));
        }

        if(Input.GetMouseButtonDown(0) && inGrid)//left clic
        {
            if (playerBoard.placeShipAt(point, ships[placingIndex]))
            {
                placingGhost = null;
            }
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
