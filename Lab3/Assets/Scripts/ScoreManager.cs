using UnityEngine;
using UnityEngine.UI; // Add this line
using static System.Net.Mime.MediaTypeNames;

public class ScoreManager : MonoBehaviour
{
    public UnityEngine.UI.Text scoreText; // Reference to the Text element

    private int score = 0;

    void Start()
    {
        UpdateScoreText();
    }

    public void IncrementScore()
    {
        score++;
        UpdateScoreText();
    }

    void UpdateScoreText()
    {
        scoreText.text = "Score: " + score;
    }
}
