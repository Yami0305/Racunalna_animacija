using UnityEngine;

public class EnemyMovement : MonoBehaviour
{
    private Vector3 targetPosition;
    private float speed;

    public ScoreManager scoreManager;
    public Canvas gameOverCanvas;

    public void Initialize(Vector3 target, float movementSpeed)
    {
        targetPosition = target;
        speed = movementSpeed;
    }

    void Update()
    {
        // Move towards the center of the circle
        transform.position = Vector3.MoveTowards(transform.position, targetPosition, speed * Time.deltaTime);

        // You can add additional logic for enemy behavior here
        if (Vector3.Distance(transform.position, targetPosition) < 0.1f)
        {
            // Trigger game over logic
            ShowGameOverScreen();
            // You might want to destroy the enemy or handle other actions here
            //Destroy(gameObject);
        }
    }

    private void ShowGameOverScreen()
    {
        // Enable the game over canvas
        if (gameOverCanvas != null)
        {
            gameOverCanvas.gameObject.SetActive(true);
        }
        Time.timeScale = 0f;
        // You can add additional logic here, such as stopping the game or other actions upon game over.
    }
}
