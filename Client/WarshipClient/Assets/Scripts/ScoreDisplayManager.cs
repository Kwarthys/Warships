using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScoreDisplayManager : MonoBehaviour
{
    public static ScoreDisplayManager instance;
    private void Awake()
    {
        instance = this;
    }

    [SerializeField]
    private GameObject playerScoreHolderPrefab;

    private List<int> playerIndexToID = new List<int>();
    private List<PlayerScoreHolderReferencer> displayTexts = new List<PlayerScoreHolderReferencer>();

    public void registerNewPlayer(int playerID, string playerName, int playerScore, bool isLocal = false)
    {
        PlayerScoreHolderReferencer holder = Instantiate(playerScoreHolderPrefab, transform).GetComponent<PlayerScoreHolderReferencer>();
        holder.scoreTextMesh.text = playerScore.ToString();
        holder.nameTextMesh.text = playerName;        

        if(isLocal)
        {
            holder.nameTextMesh.fontStyle = TMPro.FontStyles.Italic;
        }


        playerIndexToID.Add(playerID);
        displayTexts.Add(holder);
        setPlayerReadyImage(playerIndexToID.Count - 1, false);
    }

    public void setPlayerReady(int playerID, bool readyState)
    {
        setPlayerReadyImage(getPlayerIndexFromID(playerID), readyState);
    }

    public void updatePlayerScore(int playerID, int playerScore)
    {
        int index = getPlayerIndexFromID(playerID);
        displayTexts[index].scoreTextMesh.text = playerScore.ToString();
    }

    private void setPlayerReadyImage(int playerIndex, bool readyState)
    {
        displayTexts[playerIndex].readyImage.gameObject.SetActive(readyState);
        displayTexts[playerIndex].notReadyImage.gameObject.SetActive(!readyState);
    }

    private int getPlayerIndexFromID(int playerID)
    {
        for (int i = 0; i < playerIndexToID.Count; i++)
        {
            if(playerIndexToID[i] == playerID)
            {
                return i;
            }
        }

        Debug.LogError("Could not find player" + playerID);
        return -1;
    }
}
