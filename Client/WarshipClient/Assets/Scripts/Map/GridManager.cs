using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GridManager : MonoBehaviour
{
    [SerializeField]
    private Vector2Int gridSize = new Vector2Int(10,10);
    [SerializeField]
    private float cellUnitSize = 1;

    private Vector3 offset;

    private bool initialized = false;

    private GridNode[] map; //map stores -1 for water or the index of the ship located there, and if it was hit

    void Start()
    {
        map = new GridNode[gridSize.x * gridSize.y];
        offset = new Vector3(-gridSize.x / 2, 0, -gridSize.y / 2);
        for (int i = 0; i < map.Length; i++)
        {
            map[i] = new GridNode(i, new Vector2Int(i%gridSize.x, i/gridSize.x));
        }

        initialized = true;
    }

    private void OnDrawGizmos()
    {
        if (!initialized) return;

        for (int i = 0; i < map.Length; i++)
        {
            Vector2Int coords = map[i].coords;
            Gizmos.color = map[i].shipIndex == GridNode.WATER ? Color.blue : map[i].hit ? Color.red : Color.white;
            Gizmos.DrawWireCube((new Vector3(coords.x, 0, coords.y) + offset) * cellUnitSize, Vector3.one * cellUnitSize);
        }
    }
}
