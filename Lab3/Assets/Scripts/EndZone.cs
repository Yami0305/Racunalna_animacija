using UnityEngine;
using UnityEngine.UI;

public class GameOverManager : MonoBehaviour
{
    public Canvas gameOverCanvas;

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Enemy"))
        {
            // Trigger game over logic
            ShowGameOverScreen();
        }
    }

    private void ShowGameOverScreen()
    {
        // Enable the game over canvas
        gameOverCanvas.gameObject.SetActive(true);

       
    }
}
