using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GridManager : MonoBehaviour
{
    public static GridManager instance;
    private void Awake()
    {
        instance = this;
    }


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
                Vector3 point = (new Vector3(i, 0, j) + offset) * cellUnitSize;

                Instantiate(buoyPrefab, point, buoyPrefab.transform.rotation, transform);
            }
        }
    }

    private void OnDrawGizmosSelected()
    {
        if (!initialized) return;

        for (int i = 0; i < map.Length; i++)
        {
            Vector2Int coords = map[i].coords;

            Color color = Color.blue;

            if (map[i].hit) color = Color.red;

            Gizmos.color = color;
            Gizmos.DrawWireCube((new Vector3(coords.x, 0, coords.y) + offset + cellOffset) * cellUnitSize, Vector3.one * cellUnitSize);
        }
    }

    public void registerClic(Vector3 worldPos)
    {
        int hitCellIndex = fromWorldToNode(worldPos);
    }

    public Vector3 snapWorldToGrid(Vector3 worldPos)
    {
        int cellIndex = fromWorldToNode(worldPos);
        if (cellIndex == -1)
        {
            return worldPos;
        }

        Vector2Int coords = map[cellIndex].coords;
        return (new Vector3(coords.x, 0, coords.y) + offset + cellOffset) * cellUnitSize;

    }

    public int fromWorldToNode(Vector3 worldPos)
    {
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
        return coords.y * gridSize.x + coords.x;
    }
}
