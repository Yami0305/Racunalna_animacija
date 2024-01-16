using System.Diagnostics;
using UnityEngine;

public class MyGameOverManager : MonoBehaviour
{
    public Canvas gameOverCanvas;

    void Start()
    {
        UnityEngine.Debug.Log("Game Over Manager script initialized.");
    }

    void OnCollisionEnter(Collision collision)
    {
        UnityEngine.Debug.Log("Collision detected with: " + collision.gameObject.name);

        // Check if the collision involves an object with the "Enemy" tag
        if (collision.collider.CompareTag("Enemy"))
        {
            // Trigger game over logic
            ShowGameOverScreen();
        }
    }

    private void ShowGameOverScreen()
    {
        // Enable the game over canvas
        gameOverCanvas.gameObject.SetActive(true);

        // You can add additional logic here, such as stopping the game or other actions upon game over.
    }
}
