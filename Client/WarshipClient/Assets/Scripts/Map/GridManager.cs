using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GridManager : MonoBehaviour
{

    [SerializeField]
    private Vector2Int gridSize = new Vector2Int(10,10);
    [SerializeField]
    private float cellUnitSize = 1;

    [SerializeField]
    private GameObject buoyPrefab;

    private Vector3 offset;
    private Vector3 cellOffset;

    private bool initialized = false;

    private GridNode[] map; //map stores -1 for water or the index of the ship located there, and if it was hit

    void Start()
    {
        map = new GridNode[gridSize.x * gridSize.y];

        offset = new Vector3(-gridSize.x / 2f, 0, -gridSize.y / 2f);

        cellOffset = Vector3.zero;

        if (gridSize.x % 2 == 0)
        {
            cellOffset.x += 0.5f;
        }

        if (gridSize.y % 2 == 0)
        {
            cellOffset.z += 0.5f;
        }

        for (int i = 0; i < map.Length; i++)
        {
            map[i] = new GridNode(i, fromIndexToCoords(i));
        }

        initialized = true;

        spawnGridBuoys();
    }

    private void spawnGridBuoys()
    {
        for(int j = 0; j < gridSize.y + 1; ++j)
        {
            for(int i = 0; i < gridSize.x + 1; ++i)
            {
                //BottomLeftPoint
                Vector3 point = (new Vector3(i, 0, j) + offset) * cellUnitSize + transform.position;

                Instantiate(buoyPrefab, point, buoyPrefab.transform.rotation, transform);
            }
        }
    }

    private void OnDrawGizmos()
    {
        if (!initialized) return;

        for (int i = 0; i < map.Length; i++)
        {
            Vector2Int coords = map[i].coords;

            Color color = Color.blue;

            if (map[i].hit) color = Color.red;
            else if (map[i].shipIndex != GridNode.WATER) color = Color.white;

            Gizmos.color = color;
            Gizmos.DrawWireCube((new Vector3(coords.x, 0, coords.y) + offset + cellOffset) * cellUnitSize + transform.position, Vector3.one * cellUnitSize);
        }
    }

    public void registerClic(Vector3 worldPos)
    {
        int hitCellIndex = fromWorldToNode(worldPos);
    }

    public bool snapShipToGrid(Vector3 worldPos, out Vector3 point, int shipLength = 1, Ship.Orientation orientation = Ship.Orientation.NORTH)
    {
        int cellIndex = fromWorldToNode(worldPos);
        if (cellIndex == -1) //out of grid
        {
            point = worldPos;
            return false;
        }

        Vector2Int coords = map[cellIndex].coords;

        Vector3 evenOffset = Vector3.zero;
        if (shipLength % 2 == 0)
        {
            if(orientation == Ship.Orientation.NORTH || orientation == Ship.Orientation.SOUTH)
            {
                evenOffset += Vector3.forward * 0.5f;
            }
            else
            {
                evenOffset += Vector3.right * 0.5f;
            }
        }

        point = (new Vector3(coords.x, 0, coords.y) + offset + cellOffset + evenOffset) * cellUnitSize + transform.position;
        return true;
    }

    public bool placeShipAt(Vector3 worldPos, Ship ship)
    {
        int index = fromWorldToNode(worldPos);
        if (index == -1) return false;

        return checkSpaceForShip(map[index].coords, ship, true);
    }

    private bool checkSpaceForShip(Vector2Int shipCenter, Ship ship, bool placeIfEmpty = false)
    {
        RectInt shipFootprint = ship.computeShipFootprint(shipCenter);

        bool spaceFree = true;

        /*border checks*/
        if (shipFootprint.x < 0 || shipFootprint.y < 0 || shipFootprint.x + shipFootprint.width > gridSize.x || shipFootprint.y + shipFootprint.height > gridSize.y)
        {
            Debug.Log("Placement out of map : " + shipFootprint);
            return false;
        }
        /***************/

        for(int j = 0; j < shipFootprint.height && spaceFree; ++j)
        {
            for (int i = 0; i < shipFootprint.width && spaceFree; i++)
            {
                int cellIndex = fromCoordsToIndex(shipFootprint.x + i, shipFootprint.y + j);
                spaceFree = map[cellIndex].shipIndex == GridNode.WATER;

                if(!spaceFree)
                {
                    Debug.Log("A ship is already here");
                }
            }
        }

        if(spaceFree && placeIfEmpty)
        {
            for (int j = 0; j < shipFootprint.height && spaceFree; ++j)
            {
                for (int i = 0; i < shipFootprint.width && spaceFree; i++)
                {
                    int cellIndex = fromCoordsToIndex(shipFootprint.x + i, shipFootprint.y + j);
                    map[cellIndex].shipIndex = ship.index;
                }
            }
        }

        return spaceFree;
    }

    public Vector3 snapWorldToGrid(Vector3 worldPos)
    {
        int cellIndex = fromWorldToNode(worldPos);
        if (cellIndex == -1)
        {
            return worldPos;
        }

        Vector2Int coords = map[cellIndex].coords;
        return (new Vector3(coords.x, 0, coords.y) + offset + cellOffset) * cellUnitSize + transform.position;

    }

    public bool trySnapWorldToGrid(Vector3 worldPos, out Vector3 gridPos)
    {
        int cellIndex = fromWorldToNode(worldPos);

        if (cellIndex == -1)
        {
            gridPos = Vector3.zero;
            return false;
        }
        else
        {
            Vector2Int coords = map[cellIndex].coords;
            gridPos = (new Vector3(coords.x, 0, coords.y) + offset + cellOffset) * cellUnitSize + transform.position;
            return true;
        }


    }

    public int fromWorldToNode(Vector3 worldPos)
    {
        worldPos -= transform.position;

        worldPos /= cellUnitSize;
        worldPos -= offset;

        Vector2Int tempCoords = new Vector2Int((int)worldPos.x, (int)worldPos.z);
        
        if(tempCoords.x < 0 || tempCoords.y < 0 || tempCoords.x >= gridSize.x || tempCoords.y >= gridSize.y)
        {
            return -1;
        }

        int cellIndex = fromCoordsToIndex(new Vector2Int((int)worldPos.x, (int)worldPos.z));

        if(cellIndex >= 0 && cellIndex < map.Length)
        {
            return cellIndex;
        }
        else
        {
            return -1;
        }
    }

    private Vector2Int fromIndexToCoords(int index)
    {
        return new Vector2Int(index % gridSize.x, index / gridSize.x);
    }

    private int fromCoordsToIndex(Vector2Int coords)
    {
        return fromCoordsToIndex(coords.x, coords.y);
    }

    private int fromCoordsToIndex(int x, int y)
    {
        return y * gridSize.x + x;
    }
}
